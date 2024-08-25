#include "VGTrueType.h"
#include <raqm.h>
#include <freetype/ftoutln.h>
#include <fontconfig/fontconfig.h>
#include <freetype/ftstroke.h>

#ifdef OPENVG_PLATFORM_WINDOWS
#ifdef OPENVG_DEBUG_MODE
#pragma comment(lib, "brotlicommon.lib")
#pragma comment(lib, "brotlidec.lib")
#pragma comment(lib, "brotlienc.lib")
#pragma comment(lib, "bz2d.lib")
#pragma comment(lib, "charset.lib")
#pragma comment(lib, "fontconfig.lib")
#pragma comment(lib, "freetyped.lib")
#pragma comment(lib, "fribidi.lib")
#pragma comment(lib, "getopt.lib")
#pragma comment(lib, "harfbuzz.lib")
#pragma comment(lib, "harfbuzz-subset.lib")
#pragma comment(lib, "iconv.lib")
#pragma comment(lib, "intl.lib")
#pragma comment(lib, "libexpatd.lib")
#pragma comment(lib, "libpng16d.lib")
#pragma comment(lib, "libtess2.lib")
#pragma comment(lib, "pthreadVC3d.lib")
#pragma comment(lib, "pthreadVCE3d.lib")
#pragma comment(lib, "pthreadVSE3d.lib")
#pragma comment(lib, "raqm.lib")
#pragma comment(lib, "zlibd.lib")
#else
#pragma comment(lib, "brotlicommon.lib")
#pragma comment(lib, "brotlidec.lib")
#pragma comment(lib, "brotlienc.lib")
#pragma comment(lib, "bz2.lib")
#pragma comment(lib, "charset.lib")
#pragma comment(lib, "fontconfig.lib")
#pragma comment(lib, "freetype.lib")
#pragma comment(lib, "fribidi.lib")
#pragma comment(lib, "getopt.lib")
#pragma comment(lib, "harfbuzz.lib")
#pragma comment(lib, "harfbuzz-subset.lib")
#pragma comment(lib, "iconv.lib")
#pragma comment(lib, "intl.lib")
#pragma comment(lib, "libexpat.lib")
#pragma comment(lib, "libpng16.lib")
#pragma comment(lib, "libtess2.lib")
#pragma comment(lib, "pthreadVC3.lib")
#pragma comment(lib, "pthreadVCE3.lib")
#pragma comment(lib, "pthreadVSE3.lib")
#pragma comment(lib, "raqm.lib")
#pragma comment(lib, "zlib.lib")
#endif
#endif

static struct USING_FREETYPE
{
	USING_FREETYPE() { FcInit(); }
	~USING_FREETYPE() { FcFini(); }
} USING_FREETYPE;

bool VGTrueType::Path(VGTextRaw element, VGRect rect, VGString text, VGVector<VGPoint>& inPoints, VGVector<VGPointType>& inTypes)
{
	struct VGTextHash
	{
		uint32_t Group;
		uint32_t Glygh;
		uint32_t Weight;
		uint32_t Style;

		bool operator < (VGTextHash const& e) const
		{
			if (Group != e.Group) return Group < e.Group;
			if (Glygh != e.Glygh) return Glygh < e.Glygh;
			if (Weight != e.Weight) return Weight < e.Weight;
			return Style < e.Style;
		}
	};
	struct VGFontHash
	{
		uint32_t Group;
		uint32_t Weight;
		uint32_t Style;

		bool operator < (VGFontHash const& e) const
		{
			if (Group != e.Group) return Group < e.Group;
			if (Weight != e.Weight) return Weight < e.Weight;
			return Style < e.Style;
		}
	};
	struct VGTextGlygh
	{
		VGVector<VGPoint> Points;
		VGVector<VGPointType> Types;
	};
	static VGMap<VGFontHash, VGString> s_FontFileMap;
	static VGMap<VGTextHash, VGRef<VGTextGlygh>> s_FontGlyphMap;

	auto config_func = [](VGTextRaw element)->VGString {

		auto filePath = VGString();
		auto family = element->getFamily();
		auto weight = FC_WEIGHT_NORMAL;
		switch (element->getWeight())
		{
		}
		auto style = FC_SLANT_ROMAN;
		switch (element->getStyle())
		{
		case VGTextStyle::StyleNormal: style = FC_SLANT_ROMAN; break;
		case VGTextStyle::StyleItalic: style = FC_SLANT_ITALIC; break;
		case VGTextStyle::StyleOblique: style = FC_SLANT_OBLIQUE; break;
		default: style = FC_SLANT_ROMAN; break;
		}

		FcConfig* config = nullptr;
		FcPattern* pattern = nullptr;
		FcPattern* match = nullptr;

		do
		{
			// Step 2: Create a pattern for the desired font family
			pattern = FcPatternCreate();
			if (!pattern)
			{
				printf("Failed to create Fontconfig pattern\n");
				break;
			}

			// Step 3: Add properties to the pattern (e.g., font family, weight)
			FcPatternAddString(pattern, FC_FAMILY, (const FcChar8*)family.c_str());
			FcPatternAddInteger(pattern, FC_WEIGHT, weight);
			FcPatternAddInteger(pattern, FC_SLANT, style);

			// Step 4: Match the pattern to find the best matching font
			config = FcInitLoadConfigAndFonts();
			FcResult result;
			match = FcFontMatch(config, pattern, &result);
			if (!match)
			{
				printf("Failed to match the font pattern\n");
				FcPatternDestroy(pattern);
				FcConfigDestroy(config);
				break;
			}

			// Step 5: Retrieve the matched font's file path
			FcChar8* file;
			if (FcPatternGetString(match, FC_FILE, 0, &file) == FcResultMatch)
			{
				filePath = (VGCString)file;
			}
			else
			{
				printf("Failed to get font file path\n");
			}
		} while (0);

		// Cleanup
		FcPatternDestroy(pattern);
		FcPatternDestroy(match);
		FcConfigDestroy(config);

		return filePath;
		};

	auto fontPath = VGString();
	if (true)
	{
		VGFontHash key;
		key.Group = VGHash(element->getFamily().c_str());
		key.Weight = element->getWeight();
		key.Style = element->getStyle();
		auto result = s_FontFileMap.find(key);
		if (result == s_FontFileMap.end())
		{
			auto fontPath = config_func(element);
			result = s_FontFileMap.emplace(key, fontPath).first;
		}
		fontPath = result->second;
	}
	if (fontPath.empty()) return false;

	FT_Library library = nullptr;
	FT_Face face = nullptr;

	do
	{
		// Step 1: Initialize the FreeType library
		if (FT_Init_FreeType(&library))
		{
			printf("Could not initialize FreeType library\n");
			break;
		}

		// Step 2: Load the font file
		if (FT_New_Face(library, fontPath.c_str(), 0, &face))
		{
			printf("Could not load font\n");
			break;
		}

		// Step 3: Set the character size
		FT_Set_Pixel_Sizes(face, 0, 100);

		// Step 4: Load the character glyph (e.g., 'A')

		raqm_t* rq = raqm_create();
		if (rq != NULL)
		{
			raqm_direction_t dir = RAQM_DIRECTION_DEFAULT;
			switch (element->getDirection())
			{
			case VGTextDirection::DirectionLeftToRight: dir = RAQM_DIRECTION_LTR; break;
			case VGTextDirection::DirectionRightToLeft: dir = RAQM_DIRECTION_RTL; break;
			default: dir = RAQM_DIRECTION_DEFAULT; break;
			}

			if (raqm_set_text_utf8(rq, text.c_str(), text.length())
				&& raqm_set_freetype_face(rq, face)
				&& raqm_set_par_direction(rq, dir)
				&& raqm_set_language(rq, "en", 0, text.length())
				&& raqm_layout(rq))
			{
				size_t count;
				raqm_glyph_t* glyphs = raqm_get_glyphs(rq, &count);
				if (glyphs == nullptr || count == 0) continue;

				VGPoint offset;
				for (size_t i = 0; i < count; ++i)
				{
					auto glyph_index = glyphs[i].index;
					auto offsetX = glyphs[i].x_offset;
					auto offsetY = glyphs[i].y_offset;
					auto advanceX = glyphs[i].x_advance;
					auto advanceY = glyphs[i].y_advance;

					VGTextHash keyGlyph;
					keyGlyph.Group = VGHash(element->getFamily().c_str());
					keyGlyph.Glygh = glyph_index;
					keyGlyph.Weight = element->getWeight();
					keyGlyph.Style = element->getStyle();
					auto result = s_FontGlyphMap.find(keyGlyph);
					if (result == s_FontGlyphMap.end())
					{
						if (FT_Load_Glyph(face, glyph_index, FT_LOAD_NO_BITMAP))
						{
							printf("Could not load glyph\n");
							continue;
						}
						FT_GlyphSlot slot = face->glyph;
						FT_Outline& outline = slot->outline;
						auto advanceX = slot->metrics.horiAdvance;
						auto advanceY = slot->metrics.vertAdvance;

						struct VGTextGlygh2
						{
							VGVector<VGPoint>* Points;
							VGVector<VGPointType>* Types;
						} input;

						// Step 6: Process the outline
						VGVector<VGPoint> newPoints;
						VGVector<VGPointType> newTypes;

						input.Points = &newPoints;
						input.Types = &newTypes;

						auto MoveToFunction = [](const FT_Vector* to, void* user)->int {
							auto& newPoints = *((VGTextGlygh2*)user)->Points;
							auto& newTypes = *((VGTextGlygh2*)user)->Types;
							if (newPoints.size()) newTypes.emplace_back(VGPointType::Close);
							newPoints.emplace_back(to->x, to->y);
							newTypes.emplace_back(VGPointType::MoveTo);
							return 0;
							};

						auto LineToFunction = [](const FT_Vector* to, void* user)->int {
							auto& newPoints = *((VGTextGlygh2*)user)->Points;
							auto& newTypes = *((VGTextGlygh2*)user)->Types;
							newPoints.emplace_back(to->x, to->y);
							newTypes.emplace_back(VGPointType::LineTo);
							return 0;
							};

						auto ConicToFunction = [](const FT_Vector* ct, const FT_Vector* to, void* user)->int {
							auto& newPoints = *((VGTextGlygh2*)user)->Points;
							auto& newTypes = *((VGTextGlygh2*)user)->Types;
							newPoints.emplace_back(ct->x, ct->y);
							newPoints.emplace_back(to->x, to->y);
							newTypes.emplace_back(VGPointType::CurveTo);
							return 0;
							};

						auto CubicToFunction = [](const FT_Vector* ct1, const FT_Vector* ct2, const FT_Vector* to, void* user)->int {
							auto& newPoints = *((VGTextGlygh2*)user)->Points;
							auto& newTypes = *((VGTextGlygh2*)user)->Types;
							newPoints.emplace_back(ct1->x, ct1->y);
							newPoints.emplace_back(ct2->x, ct2->y);
							newPoints.emplace_back(to->x, to->y);
							newTypes.emplace_back(VGPointType::CubicTo);
							return 0;
							};

						FT_Outline_Funcs callbacks;
						callbacks.move_to = MoveToFunction;
						callbacks.line_to = LineToFunction;
						callbacks.conic_to = ConicToFunction;
						callbacks.cubic_to = CubicToFunction;
						callbacks.shift = 0;
						callbacks.delta = 0;

						if (FT_Outline_Decompose(&outline, &callbacks, &input))
						{
							printf("Could not load glyph\n");
							continue;
						}

						for (size_t k = 0; k < newPoints.size(); ++k)
						{
							newPoints[k].Y = advanceY - newPoints[k].Y;
						}

						auto value = VGNew<VGTextGlygh>();
						value->Points = newPoints; value->Types = newTypes;
						s_FontGlyphMap.emplace(keyGlyph, value);
						result = s_FontGlyphMap.find(keyGlyph);
					}

					constexpr auto FT_SCALE = 1.0f / 64;
					auto& points = result->second->Points;
					auto& types = result->second->Types;

					for (size_t k = 0; k < points.size(); ++k)
					{
						inPoints.push_back({ (offset.X + offsetX + points[k].X) * FT_SCALE, (offset.Y + offsetY + points[k].Y) * FT_SCALE });
					}
					for (size_t k = 0; k < types.size(); ++k)
					{
						inTypes.push_back(types[k]);
					}
					inTypes.emplace_back(VGPointType::Close);
					offset.X += advanceX; offset.Y += advanceY;
				}
			}

			raqm_destroy(rq);
		}

		// Clean up
		FT_Done_Face(face);
		FT_Done_FreeType(library);
		return true;

	} while (0);

	// Clean up
	FT_Done_Face(face);
	FT_Done_FreeType(library);
	return false;
}
