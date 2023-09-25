#pragma once
#include<yuki/unicode_base.hpp>

namespace yuki::unicode{

inline constexpr yuki::CInterval<char32_t> ASCII[1] = {0x0,0x7F};
inline constexpr yuki::CInterval<char32_t> Latin_1_Sup[1] = {0x80,0xFF};
inline constexpr yuki::CInterval<char32_t> Latin_Ext_A[1] = {0x100,0x17F};
inline constexpr yuki::CInterval<char32_t> Latin_Ext_B[1] = {0x180,0x24F};
inline constexpr yuki::CInterval<char32_t> IPA_Ext[1] = {0x250,0x2AF};
inline constexpr yuki::CInterval<char32_t> Modifier_Letters[1] = {0x2B0,0x2FF};
inline constexpr yuki::CInterval<char32_t> Diacriticals[1] = {0x300,0x36F};
inline constexpr yuki::CInterval<char32_t> Greek[1] = {0x370,0x3FF};
inline constexpr yuki::CInterval<char32_t> Cyrillic[1] = {0x400,0x4FF};
inline constexpr yuki::CInterval<char32_t> Cyrillic_Sup[1] = {0x500,0x52F};
inline constexpr yuki::CInterval<char32_t> Armenian[1] = {0x530,0x58F};
inline constexpr yuki::CInterval<char32_t> Hebrew[1] = {0x590,0x5FF};
inline constexpr yuki::CInterval<char32_t> Arabic[1] = {0x600,0x6FF};
inline constexpr yuki::CInterval<char32_t> Syriac[1] = {0x700,0x74F};
inline constexpr yuki::CInterval<char32_t> Arabic_Sup[1] = {0x750,0x77F};
inline constexpr yuki::CInterval<char32_t> Thaana[1] = {0x780,0x7BF};
inline constexpr yuki::CInterval<char32_t> NKo[1] = {0x7C0,0x7FF};
inline constexpr yuki::CInterval<char32_t> Samaritan[1] = {0x800,0x83F};
inline constexpr yuki::CInterval<char32_t> Mandaic[1] = {0x840,0x85F};
inline constexpr yuki::CInterval<char32_t> Syriac_Sup[1] = {0x860,0x86F};
inline constexpr yuki::CInterval<char32_t> Arabic_Ext_B[1] = {0x870,0x89F};
inline constexpr yuki::CInterval<char32_t> Arabic_Ext_A[1] = {0x8A0,0x8FF};
inline constexpr yuki::CInterval<char32_t> Devanagari[1] = {0x900,0x97F};
inline constexpr yuki::CInterval<char32_t> Bengali[1] = {0x980,0x9FF};
inline constexpr yuki::CInterval<char32_t> Gurmukhi[1] = {0xA00,0xA7F};
inline constexpr yuki::CInterval<char32_t> Gujarati[1] = {0xA80,0xAFF};
inline constexpr yuki::CInterval<char32_t> Oriya[1] = {0xB00,0xB7F};
inline constexpr yuki::CInterval<char32_t> Tamil[1] = {0xB80,0xBFF};
inline constexpr yuki::CInterval<char32_t> Telugu[1] = {0xC00,0xC7F};
inline constexpr yuki::CInterval<char32_t> Kannada[1] = {0xC80,0xCFF};
inline constexpr yuki::CInterval<char32_t> Malayalam[1] = {0xD00,0xD7F};
inline constexpr yuki::CInterval<char32_t> Sinhala[1] = {0xD80,0xDFF};
inline constexpr yuki::CInterval<char32_t> Thai_blk[1] = {0xE00,0xE7F};
inline constexpr yuki::CInterval<char32_t> Lao[1] = {0xE80,0xEFF};
inline constexpr yuki::CInterval<char32_t> Tibetan[1] = {0xF00,0xFFF};
inline constexpr yuki::CInterval<char32_t> Myanmar[1] = {0x1000,0x109F};
inline constexpr yuki::CInterval<char32_t> Georgian[1] = {0x10A0,0x10FF};
inline constexpr yuki::CInterval<char32_t> Jamo[1] = {0x1100,0x11FF};
inline constexpr yuki::CInterval<char32_t> Ethiopic[1] = {0x1200,0x137F};
inline constexpr yuki::CInterval<char32_t> Ethiopic_Sup[1] = {0x1380,0x139F};
inline constexpr yuki::CInterval<char32_t> Cherokee[1] = {0x13A0,0x13FF};
inline constexpr yuki::CInterval<char32_t> UCAS[1] = {0x1400,0x167F};
inline constexpr yuki::CInterval<char32_t> Ogham[1] = {0x1680,0x169F};
inline constexpr yuki::CInterval<char32_t> Runic[1] = {0x16A0,0x16FF};
inline constexpr yuki::CInterval<char32_t> Tagalog[1] = {0x1700,0x171F};
inline constexpr yuki::CInterval<char32_t> Hanunoo[1] = {0x1720,0x173F};
inline constexpr yuki::CInterval<char32_t> Buhid[1] = {0x1740,0x175F};
inline constexpr yuki::CInterval<char32_t> Tagbanwa[1] = {0x1760,0x177F};
inline constexpr yuki::CInterval<char32_t> Khmer[1] = {0x1780,0x17FF};
inline constexpr yuki::CInterval<char32_t> Mongolian[1] = {0x1800,0x18AF};
inline constexpr yuki::CInterval<char32_t> UCAS_Ext[1] = {0x18B0,0x18FF};
inline constexpr yuki::CInterval<char32_t> Limbu[1] = {0x1900,0x194F};
inline constexpr yuki::CInterval<char32_t> Tai_Le[1] = {0x1950,0x197F};
inline constexpr yuki::CInterval<char32_t> New_Tai_Lue[1] = {0x1980,0x19DF};
inline constexpr yuki::CInterval<char32_t> Khmer_Symbols[1] = {0x19E0,0x19FF};
inline constexpr yuki::CInterval<char32_t> Buginese[1] = {0x1A00,0x1A1F};
inline constexpr yuki::CInterval<char32_t> Tai_Tham[1] = {0x1A20,0x1AAF};
inline constexpr yuki::CInterval<char32_t> Diacriticals_Ext[1] = {0x1AB0,0x1AFF};
inline constexpr yuki::CInterval<char32_t> Balinese[1] = {0x1B00,0x1B7F};
inline constexpr yuki::CInterval<char32_t> Sundanese[1] = {0x1B80,0x1BBF};
inline constexpr yuki::CInterval<char32_t> Batak[1] = {0x1BC0,0x1BFF};
inline constexpr yuki::CInterval<char32_t> Lepcha[1] = {0x1C00,0x1C4F};
inline constexpr yuki::CInterval<char32_t> Ol_Chiki[1] = {0x1C50,0x1C7F};
inline constexpr yuki::CInterval<char32_t> Cyrillic_Ext_C[1] = {0x1C80,0x1C8F};
inline constexpr yuki::CInterval<char32_t> Georgian_Ext[1] = {0x1C90,0x1CBF};
inline constexpr yuki::CInterval<char32_t> Sundanese_Sup[1] = {0x1CC0,0x1CCF};
inline constexpr yuki::CInterval<char32_t> Vedic_Ext[1] = {0x1CD0,0x1CFF};
inline constexpr yuki::CInterval<char32_t> Phonetic_Ext[1] = {0x1D00,0x1D7F};
inline constexpr yuki::CInterval<char32_t> Phonetic_Ext_Sup[1] = {0x1D80,0x1DBF};
inline constexpr yuki::CInterval<char32_t> Diacriticals_Sup[1] = {0x1DC0,0x1DFF};
inline constexpr yuki::CInterval<char32_t> Latin_Ext_Additional[1] = {0x1E00,0x1EFF};
inline constexpr yuki::CInterval<char32_t> Greek_Ext[1] = {0x1F00,0x1FFF};
inline constexpr yuki::CInterval<char32_t> Punctuation[1] = {0x2000,0x206F};
inline constexpr yuki::CInterval<char32_t> Super_And_Sub[1] = {0x2070,0x209F};
inline constexpr yuki::CInterval<char32_t> Currency_Symbols[1] = {0x20A0,0x20CF};
inline constexpr yuki::CInterval<char32_t> Diacriticals_For_Symbols[1] = {0x20D0,0x20FF};
inline constexpr yuki::CInterval<char32_t> Letterlike_Symbols[1] = {0x2100,0x214F};
inline constexpr yuki::CInterval<char32_t> Number_Forms[1] = {0x2150,0x218F};
inline constexpr yuki::CInterval<char32_t> Arrows[1] = {0x2190,0x21FF};
inline constexpr yuki::CInterval<char32_t> Math_Operators[1] = {0x2200,0x22FF};
inline constexpr yuki::CInterval<char32_t> Misc_Technical[1] = {0x2300,0x23FF};
inline constexpr yuki::CInterval<char32_t> Control_Pictures[1] = {0x2400,0x243F};
inline constexpr yuki::CInterval<char32_t> OCR[1] = {0x2440,0x245F};
inline constexpr yuki::CInterval<char32_t> Enclosed_Alphanum[1] = {0x2460,0x24FF};
inline constexpr yuki::CInterval<char32_t> Box_Drawing[1] = {0x2500,0x257F};
inline constexpr yuki::CInterval<char32_t> Block_Elements[1] = {0x2580,0x259F};
inline constexpr yuki::CInterval<char32_t> Geometric_Shapes[1] = {0x25A0,0x25FF};
inline constexpr yuki::CInterval<char32_t> Misc_Symbols[1] = {0x2600,0x26FF};
inline constexpr yuki::CInterval<char32_t> Dingbats[1] = {0x2700,0x27BF};
inline constexpr yuki::CInterval<char32_t> Misc_Math_Symbols_A[1] = {0x27C0,0x27EF};
inline constexpr yuki::CInterval<char32_t> Sup_Arrows_A[1] = {0x27F0,0x27FF};
inline constexpr yuki::CInterval<char32_t> Braille[1] = {0x2800,0x28FF};
inline constexpr yuki::CInterval<char32_t> Sup_Arrows_B[1] = {0x2900,0x297F};
inline constexpr yuki::CInterval<char32_t> Misc_Math_Symbols_B[1] = {0x2980,0x29FF};
inline constexpr yuki::CInterval<char32_t> Sup_Math_Operators[1] = {0x2A00,0x2AFF};
inline constexpr yuki::CInterval<char32_t> Misc_Arrows[1] = {0x2B00,0x2BFF};
inline constexpr yuki::CInterval<char32_t> Glagolitic[1] = {0x2C00,0x2C5F};
inline constexpr yuki::CInterval<char32_t> Latin_Ext_C[1] = {0x2C60,0x2C7F};
inline constexpr yuki::CInterval<char32_t> Coptic[1] = {0x2C80,0x2CFF};
inline constexpr yuki::CInterval<char32_t> Georgian_Sup[1] = {0x2D00,0x2D2F};
inline constexpr yuki::CInterval<char32_t> Tifinagh[1] = {0x2D30,0x2D7F};
inline constexpr yuki::CInterval<char32_t> Ethiopic_Ext[1] = {0x2D80,0x2DDF};
inline constexpr yuki::CInterval<char32_t> Cyrillic_Ext_A[1] = {0x2DE0,0x2DFF};
inline constexpr yuki::CInterval<char32_t> Sup_Punctuation[1] = {0x2E00,0x2E7F};
inline constexpr yuki::CInterval<char32_t> CJK_Radicals_Sup[1] = {0x2E80,0x2EFF};
inline constexpr yuki::CInterval<char32_t> Kangxi[1] = {0x2F00,0x2FDF};
inline constexpr yuki::CInterval<char32_t> IDC_blk[1] = {0x2FF0,0x2FFF};
inline constexpr yuki::CInterval<char32_t> CJK_Symbols[1] = {0x3000,0x303F};
inline constexpr yuki::CInterval<char32_t> Hiragana[1] = {0x3040,0x309F};
inline constexpr yuki::CInterval<char32_t> Katakana[1] = {0x30A0,0x30FF};
inline constexpr yuki::CInterval<char32_t> Bopomofo[1] = {0x3100,0x312F};
inline constexpr yuki::CInterval<char32_t> Compat_Jamo[1] = {0x3130,0x318F};
inline constexpr yuki::CInterval<char32_t> Kanbun[1] = {0x3190,0x319F};
inline constexpr yuki::CInterval<char32_t> Bopomofo_Ext[1] = {0x31A0,0x31BF};
inline constexpr yuki::CInterval<char32_t> CJK_Strokes[1] = {0x31C0,0x31EF};
inline constexpr yuki::CInterval<char32_t> Katakana_Ext[1] = {0x31F0,0x31FF};
inline constexpr yuki::CInterval<char32_t> Enclosed_CJK[1] = {0x3200,0x32FF};
inline constexpr yuki::CInterval<char32_t> CJK_Compat[1] = {0x3300,0x33FF};
inline constexpr yuki::CInterval<char32_t> CJK_Ext_A[1] = {0x3400,0x4DBF};
inline constexpr yuki::CInterval<char32_t> Yijing[1] = {0x4DC0,0x4DFF};
inline constexpr yuki::CInterval<char32_t> CJK[1] = {0x4E00,0x9FFF};
inline constexpr yuki::CInterval<char32_t> Yi_Syllables[1] = {0xA000,0xA48F};
inline constexpr yuki::CInterval<char32_t> Yi_Radicals[1] = {0xA490,0xA4CF};
inline constexpr yuki::CInterval<char32_t> Lisu_blk[1] = {0xA4D0,0xA4FF};
inline constexpr yuki::CInterval<char32_t> Vai[1] = {0xA500,0xA63F};
inline constexpr yuki::CInterval<char32_t> Cyrillic_Ext_B[1] = {0xA640,0xA69F};
inline constexpr yuki::CInterval<char32_t> Bamum[1] = {0xA6A0,0xA6FF};
inline constexpr yuki::CInterval<char32_t> Modifier_Tone_Letters[1] = {0xA700,0xA71F};
inline constexpr yuki::CInterval<char32_t> Latin_Ext_D[1] = {0xA720,0xA7FF};
inline constexpr yuki::CInterval<char32_t> Syloti_Nagri[1] = {0xA800,0xA82F};
inline constexpr yuki::CInterval<char32_t> Indic_Number_Forms[1] = {0xA830,0xA83F};
inline constexpr yuki::CInterval<char32_t> Phags_Pa[1] = {0xA840,0xA87F};
inline constexpr yuki::CInterval<char32_t> Saurashtra[1] = {0xA880,0xA8DF};
inline constexpr yuki::CInterval<char32_t> Devanagari_Ext[1] = {0xA8E0,0xA8FF};
inline constexpr yuki::CInterval<char32_t> Kayah_Li[1] = {0xA900,0xA92F};
inline constexpr yuki::CInterval<char32_t> Rejang[1] = {0xA930,0xA95F};
inline constexpr yuki::CInterval<char32_t> Jamo_Ext_A[1] = {0xA960,0xA97F};
inline constexpr yuki::CInterval<char32_t> Javanese[1] = {0xA980,0xA9DF};
inline constexpr yuki::CInterval<char32_t> Myanmar_Ext_B[1] = {0xA9E0,0xA9FF};
inline constexpr yuki::CInterval<char32_t> Cham_blk[1] = {0xAA00,0xAA5F};
inline constexpr yuki::CInterval<char32_t> Myanmar_Ext_A[1] = {0xAA60,0xAA7F};
inline constexpr yuki::CInterval<char32_t> Tai_Viet[1] = {0xAA80,0xAADF};
inline constexpr yuki::CInterval<char32_t> Meetei_Mayek_Ext[1] = {0xAAE0,0xAAFF};
inline constexpr yuki::CInterval<char32_t> Ethiopic_Ext_A[1] = {0xAB00,0xAB2F};
inline constexpr yuki::CInterval<char32_t> Latin_Ext_E[1] = {0xAB30,0xAB6F};
inline constexpr yuki::CInterval<char32_t> Cherokee_Sup[1] = {0xAB70,0xABBF};
inline constexpr yuki::CInterval<char32_t> Meetei_Mayek[1] = {0xABC0,0xABFF};
inline constexpr yuki::CInterval<char32_t> Hangul[1] = {0xAC00,0xD7AF};
inline constexpr yuki::CInterval<char32_t> Jamo_Ext_B[1] = {0xD7B0,0xD7FF};
inline constexpr yuki::CInterval<char32_t> High_Surrogates[1] = {0xD800,0xDB7F};
inline constexpr yuki::CInterval<char32_t> High_PU_Surrogates[1] = {0xDB80,0xDBFF};
inline constexpr yuki::CInterval<char32_t> Low_Surrogates[1] = {0xDC00,0xDFFF};
inline constexpr yuki::CInterval<char32_t> PUA[1] = {0xE000,0xF8FF};
inline constexpr yuki::CInterval<char32_t> CJK_Compat_Ideographs[1] = {0xF900,0xFAFF};
inline constexpr yuki::CInterval<char32_t> Alphabetic_PF[1] = {0xFB00,0xFB4F};
inline constexpr yuki::CInterval<char32_t> Arabic_PF_A[1] = {0xFB50,0xFDFF};
inline constexpr yuki::CInterval<char32_t> VS_blk[1] = {0xFE00,0xFE0F};
inline constexpr yuki::CInterval<char32_t> Vertical_Forms[1] = {0xFE10,0xFE1F};
inline constexpr yuki::CInterval<char32_t> Half_Marks[1] = {0xFE20,0xFE2F};
inline constexpr yuki::CInterval<char32_t> CJK_Compat_Forms[1] = {0xFE30,0xFE4F};
inline constexpr yuki::CInterval<char32_t> Small_Forms[1] = {0xFE50,0xFE6F};
inline constexpr yuki::CInterval<char32_t> Arabic_PF_B[1] = {0xFE70,0xFEFF};
inline constexpr yuki::CInterval<char32_t> Half_And_Full_Forms[1] = {0xFF00,0xFFEF};
inline constexpr yuki::CInterval<char32_t> Specials[1] = {0xFFF0,0xFFFF};
inline constexpr yuki::CInterval<char32_t> Linear_B_Syllabary[1] = {0x10000,0x1007F};
inline constexpr yuki::CInterval<char32_t> Linear_B_Ideograms[1] = {0x10080,0x100FF};
inline constexpr yuki::CInterval<char32_t> Aegean_Numbers[1] = {0x10100,0x1013F};
inline constexpr yuki::CInterval<char32_t> Ancient_Greek_Numbers[1] = {0x10140,0x1018F};
inline constexpr yuki::CInterval<char32_t> Ancient_Symbols[1] = {0x10190,0x101CF};
inline constexpr yuki::CInterval<char32_t> Phaistos[1] = {0x101D0,0x101FF};
inline constexpr yuki::CInterval<char32_t> Lycian[1] = {0x10280,0x1029F};
inline constexpr yuki::CInterval<char32_t> Carian[1] = {0x102A0,0x102DF};
inline constexpr yuki::CInterval<char32_t> Coptic_Epact_Numbers[1] = {0x102E0,0x102FF};
inline constexpr yuki::CInterval<char32_t> Old_Italic[1] = {0x10300,0x1032F};
inline constexpr yuki::CInterval<char32_t> Gothic[1] = {0x10330,0x1034F};
inline constexpr yuki::CInterval<char32_t> Old_Permic[1] = {0x10350,0x1037F};
inline constexpr yuki::CInterval<char32_t> Ugaritic[1] = {0x10380,0x1039F};
inline constexpr yuki::CInterval<char32_t> Old_Persian[1] = {0x103A0,0x103DF};
inline constexpr yuki::CInterval<char32_t> Deseret[1] = {0x10400,0x1044F};
inline constexpr yuki::CInterval<char32_t> Shavian[1] = {0x10450,0x1047F};
inline constexpr yuki::CInterval<char32_t> Osmanya[1] = {0x10480,0x104AF};
inline constexpr yuki::CInterval<char32_t> Osage[1] = {0x104B0,0x104FF};
inline constexpr yuki::CInterval<char32_t> Elbasan[1] = {0x10500,0x1052F};
inline constexpr yuki::CInterval<char32_t> Caucasian_Albanian[1] = {0x10530,0x1056F};
inline constexpr yuki::CInterval<char32_t> Vithkuqi[1] = {0x10570,0x105BF};
inline constexpr yuki::CInterval<char32_t> Linear_A[1] = {0x10600,0x1077F};
inline constexpr yuki::CInterval<char32_t> Latin_Ext_F[1] = {0x10780,0x107BF};
inline constexpr yuki::CInterval<char32_t> Cypriot_Syllabary[1] = {0x10800,0x1083F};
inline constexpr yuki::CInterval<char32_t> Imperial_Aramaic[1] = {0x10840,0x1085F};
inline constexpr yuki::CInterval<char32_t> Palmyrene[1] = {0x10860,0x1087F};
inline constexpr yuki::CInterval<char32_t> Nabataean[1] = {0x10880,0x108AF};
inline constexpr yuki::CInterval<char32_t> Hatran[1] = {0x108E0,0x108FF};
inline constexpr yuki::CInterval<char32_t> Phoenician[1] = {0x10900,0x1091F};
inline constexpr yuki::CInterval<char32_t> Lydian[1] = {0x10920,0x1093F};
inline constexpr yuki::CInterval<char32_t> Meroitic_Hieroglyphs[1] = {0x10980,0x1099F};
inline constexpr yuki::CInterval<char32_t> Meroitic_Cursive[1] = {0x109A0,0x109FF};
inline constexpr yuki::CInterval<char32_t> Kharoshthi[1] = {0x10A00,0x10A5F};
inline constexpr yuki::CInterval<char32_t> Old_South_Arabian[1] = {0x10A60,0x10A7F};
inline constexpr yuki::CInterval<char32_t> Old_North_Arabian[1] = {0x10A80,0x10A9F};
inline constexpr yuki::CInterval<char32_t> Manichaean[1] = {0x10AC0,0x10AFF};
inline constexpr yuki::CInterval<char32_t> Avestan[1] = {0x10B00,0x10B3F};
inline constexpr yuki::CInterval<char32_t> Inscriptional_Parthian[1] = {0x10B40,0x10B5F};
inline constexpr yuki::CInterval<char32_t> Inscriptional_Pahlavi[1] = {0x10B60,0x10B7F};
inline constexpr yuki::CInterval<char32_t> Psalter_Pahlavi[1] = {0x10B80,0x10BAF};
inline constexpr yuki::CInterval<char32_t> Old_Turkic[1] = {0x10C00,0x10C4F};
inline constexpr yuki::CInterval<char32_t> Old_Hungarian[1] = {0x10C80,0x10CFF};
inline constexpr yuki::CInterval<char32_t> Hanifi_Rohingya[1] = {0x10D00,0x10D3F};
inline constexpr yuki::CInterval<char32_t> Rumi[1] = {0x10E60,0x10E7F};
inline constexpr yuki::CInterval<char32_t> Yezidi[1] = {0x10E80,0x10EBF};
inline constexpr yuki::CInterval<char32_t> Arabic_Ext_C[1] = {0x10EC0,0x10EFF};
inline constexpr yuki::CInterval<char32_t> Old_Sogdian[1] = {0x10F00,0x10F2F};
inline constexpr yuki::CInterval<char32_t> Sogdian[1] = {0x10F30,0x10F6F};
inline constexpr yuki::CInterval<char32_t> Old_Uyghur[1] = {0x10F70,0x10FAF};
inline constexpr yuki::CInterval<char32_t> Chorasmian[1] = {0x10FB0,0x10FDF};
inline constexpr yuki::CInterval<char32_t> Elymaic[1] = {0x10FE0,0x10FFF};
inline constexpr yuki::CInterval<char32_t> Brahmi[1] = {0x11000,0x1107F};
inline constexpr yuki::CInterval<char32_t> Kaithi[1] = {0x11080,0x110CF};
inline constexpr yuki::CInterval<char32_t> Sora_Sompeng[1] = {0x110D0,0x110FF};
inline constexpr yuki::CInterval<char32_t> Chakma[1] = {0x11100,0x1114F};
inline constexpr yuki::CInterval<char32_t> Mahajani[1] = {0x11150,0x1117F};
inline constexpr yuki::CInterval<char32_t> Sharada[1] = {0x11180,0x111DF};
inline constexpr yuki::CInterval<char32_t> Sinhala_Archaic_Numbers[1] = {0x111E0,0x111FF};
inline constexpr yuki::CInterval<char32_t> Khojki[1] = {0x11200,0x1124F};
inline constexpr yuki::CInterval<char32_t> Multani[1] = {0x11280,0x112AF};
inline constexpr yuki::CInterval<char32_t> Khudawadi[1] = {0x112B0,0x112FF};
inline constexpr yuki::CInterval<char32_t> Grantha[1] = {0x11300,0x1137F};
inline constexpr yuki::CInterval<char32_t> Newa_blk[1] = {0x11400,0x1147F};
inline constexpr yuki::CInterval<char32_t> Tirhuta[1] = {0x11480,0x114DF};
inline constexpr yuki::CInterval<char32_t> Siddham[1] = {0x11580,0x115FF};
inline constexpr yuki::CInterval<char32_t> Modi_blk[1] = {0x11600,0x1165F};
inline constexpr yuki::CInterval<char32_t> Mongolian_Sup[1] = {0x11660,0x1167F};
inline constexpr yuki::CInterval<char32_t> Takri[1] = {0x11680,0x116CF};
inline constexpr yuki::CInterval<char32_t> Ahom_blk[1] = {0x11700,0x1174F};
inline constexpr yuki::CInterval<char32_t> Dogra[1] = {0x11800,0x1184F};
inline constexpr yuki::CInterval<char32_t> Warang_Citi[1] = {0x118A0,0x118FF};
inline constexpr yuki::CInterval<char32_t> Dives_Akuru[1] = {0x11900,0x1195F};
inline constexpr yuki::CInterval<char32_t> Nandinagari[1] = {0x119A0,0x119FF};
inline constexpr yuki::CInterval<char32_t> Zanabazar_Square[1] = {0x11A00,0x11A4F};
inline constexpr yuki::CInterval<char32_t> Soyombo[1] = {0x11A50,0x11AAF};
inline constexpr yuki::CInterval<char32_t> UCAS_Ext_A[1] = {0x11AB0,0x11ABF};
inline constexpr yuki::CInterval<char32_t> Pau_Cin_Hau[1] = {0x11AC0,0x11AFF};
inline constexpr yuki::CInterval<char32_t> Devanagari_Ext_A[1] = {0x11B00,0x11B5F};
inline constexpr yuki::CInterval<char32_t> Bhaiksuki[1] = {0x11C00,0x11C6F};
inline constexpr yuki::CInterval<char32_t> Marchen[1] = {0x11C70,0x11CBF};
inline constexpr yuki::CInterval<char32_t> Masaram_Gondi[1] = {0x11D00,0x11D5F};
inline constexpr yuki::CInterval<char32_t> Gunjala_Gondi[1] = {0x11D60,0x11DAF};
inline constexpr yuki::CInterval<char32_t> Makasar[1] = {0x11EE0,0x11EFF};
inline constexpr yuki::CInterval<char32_t> Kawi_blk[1] = {0x11F00,0x11F5F};
inline constexpr yuki::CInterval<char32_t> Lisu_Sup[1] = {0x11FB0,0x11FBF};
inline constexpr yuki::CInterval<char32_t> Tamil_Sup[1] = {0x11FC0,0x11FFF};
inline constexpr yuki::CInterval<char32_t> Cuneiform[1] = {0x12000,0x123FF};
inline constexpr yuki::CInterval<char32_t> Cuneiform_Numbers[1] = {0x12400,0x1247F};
inline constexpr yuki::CInterval<char32_t> Early_Dynastic_Cuneiform[1] = {0x12480,0x1254F};
inline constexpr yuki::CInterval<char32_t> Cypro_Minoan[1] = {0x12F90,0x12FFF};
inline constexpr yuki::CInterval<char32_t> Egyptian_Hieroglyphs[1] = {0x13000,0x1342F};
inline constexpr yuki::CInterval<char32_t> Egyptian_Hieroglyph_Format_Controls[1] = {0x13430,0x1345F};
inline constexpr yuki::CInterval<char32_t> Anatolian_Hieroglyphs[1] = {0x14400,0x1467F};
inline constexpr yuki::CInterval<char32_t> Bamum_Sup[1] = {0x16800,0x16A3F};
inline constexpr yuki::CInterval<char32_t> Mro[1] = {0x16A40,0x16A6F};
inline constexpr yuki::CInterval<char32_t> Tangsa[1] = {0x16A70,0x16ACF};
inline constexpr yuki::CInterval<char32_t> Bassa_Vah[1] = {0x16AD0,0x16AFF};
inline constexpr yuki::CInterval<char32_t> Pahawh_Hmong[1] = {0x16B00,0x16B8F};
inline constexpr yuki::CInterval<char32_t> Medefaidrin[1] = {0x16E40,0x16E9F};
inline constexpr yuki::CInterval<char32_t> Miao[1] = {0x16F00,0x16F9F};
inline constexpr yuki::CInterval<char32_t> Ideographic_Symbols[1] = {0x16FE0,0x16FFF};
inline constexpr yuki::CInterval<char32_t> Tangut[1] = {0x17000,0x187FF};
inline constexpr yuki::CInterval<char32_t> Tangut_Components[1] = {0x18800,0x18AFF};
inline constexpr yuki::CInterval<char32_t> Khitan_Small_Script[1] = {0x18B00,0x18CFF};
inline constexpr yuki::CInterval<char32_t> Tangut_Sup[1] = {0x18D00,0x18D7F};
inline constexpr yuki::CInterval<char32_t> Kana_Ext_B[1] = {0x1AFF0,0x1AFFF};
inline constexpr yuki::CInterval<char32_t> Kana_Sup[1] = {0x1B000,0x1B0FF};
inline constexpr yuki::CInterval<char32_t> Kana_Ext_A[1] = {0x1B100,0x1B12F};
inline constexpr yuki::CInterval<char32_t> Small_Kana_Ext[1] = {0x1B130,0x1B16F};
inline constexpr yuki::CInterval<char32_t> Nushu[1] = {0x1B170,0x1B2FF};
inline constexpr yuki::CInterval<char32_t> Duployan[1] = {0x1BC00,0x1BC9F};
inline constexpr yuki::CInterval<char32_t> Shorthand_Format_Controls[1] = {0x1BCA0,0x1BCAF};
inline constexpr yuki::CInterval<char32_t> Znamenny_Music[1] = {0x1CF00,0x1CFCF};
inline constexpr yuki::CInterval<char32_t> Byzantine_Music[1] = {0x1D000,0x1D0FF};
inline constexpr yuki::CInterval<char32_t> Music[1] = {0x1D100,0x1D1FF};
inline constexpr yuki::CInterval<char32_t> Ancient_Greek_Music[1] = {0x1D200,0x1D24F};
inline constexpr yuki::CInterval<char32_t> Kaktovik_Numerals[1] = {0x1D2C0,0x1D2DF};
inline constexpr yuki::CInterval<char32_t> Mayan_Numerals[1] = {0x1D2E0,0x1D2FF};
inline constexpr yuki::CInterval<char32_t> Tai_Xuan_Jing[1] = {0x1D300,0x1D35F};
inline constexpr yuki::CInterval<char32_t> Counting_Rod[1] = {0x1D360,0x1D37F};
inline constexpr yuki::CInterval<char32_t> Math_Alphanum[1] = {0x1D400,0x1D7FF};
inline constexpr yuki::CInterval<char32_t> Sutton_SignWriting[1] = {0x1D800,0x1DAAF};
inline constexpr yuki::CInterval<char32_t> Latin_Ext_G[1] = {0x1DF00,0x1DFFF};
inline constexpr yuki::CInterval<char32_t> Glagolitic_Sup[1] = {0x1E000,0x1E02F};
inline constexpr yuki::CInterval<char32_t> Cyrillic_Ext_D[1] = {0x1E030,0x1E08F};
inline constexpr yuki::CInterval<char32_t> Nyiakeng_Puachue_Hmong[1] = {0x1E100,0x1E14F};
inline constexpr yuki::CInterval<char32_t> Toto_blk[1] = {0x1E290,0x1E2BF};
inline constexpr yuki::CInterval<char32_t> Wancho[1] = {0x1E2C0,0x1E2FF};
inline constexpr yuki::CInterval<char32_t> Nag_Mundari[1] = {0x1E4D0,0x1E4FF};
inline constexpr yuki::CInterval<char32_t> Ethiopic_Ext_B[1] = {0x1E7E0,0x1E7FF};
inline constexpr yuki::CInterval<char32_t> Mende_Kikakui[1] = {0x1E800,0x1E8DF};
inline constexpr yuki::CInterval<char32_t> Adlam[1] = {0x1E900,0x1E95F};
inline constexpr yuki::CInterval<char32_t> Indic_Siyaq_Numbers[1] = {0x1EC70,0x1ECBF};
inline constexpr yuki::CInterval<char32_t> Ottoman_Siyaq_Numbers[1] = {0x1ED00,0x1ED4F};
inline constexpr yuki::CInterval<char32_t> Arabic_Math[1] = {0x1EE00,0x1EEFF};
inline constexpr yuki::CInterval<char32_t> Mahjong[1] = {0x1F000,0x1F02F};
inline constexpr yuki::CInterval<char32_t> Domino[1] = {0x1F030,0x1F09F};
inline constexpr yuki::CInterval<char32_t> Playing_Cards[1] = {0x1F0A0,0x1F0FF};
inline constexpr yuki::CInterval<char32_t> Enclosed_Alphanum_Sup[1] = {0x1F100,0x1F1FF};
inline constexpr yuki::CInterval<char32_t> Enclosed_Ideographic_Sup[1] = {0x1F200,0x1F2FF};
inline constexpr yuki::CInterval<char32_t> Misc_Pictographs[1] = {0x1F300,0x1F5FF};
inline constexpr yuki::CInterval<char32_t> Emoticons[1] = {0x1F600,0x1F64F};
inline constexpr yuki::CInterval<char32_t> Ornamental_Dingbats[1] = {0x1F650,0x1F67F};
inline constexpr yuki::CInterval<char32_t> Transport_And_Map[1] = {0x1F680,0x1F6FF};
inline constexpr yuki::CInterval<char32_t> Alchemical[1] = {0x1F700,0x1F77F};
inline constexpr yuki::CInterval<char32_t> Geometric_Shapes_Ext[1] = {0x1F780,0x1F7FF};
inline constexpr yuki::CInterval<char32_t> Sup_Arrows_C[1] = {0x1F800,0x1F8FF};
inline constexpr yuki::CInterval<char32_t> Sup_Symbols_And_Pictographs[1] = {0x1F900,0x1F9FF};
inline constexpr yuki::CInterval<char32_t> Chess_Symbols[1] = {0x1FA00,0x1FA6F};
inline constexpr yuki::CInterval<char32_t> Symbols_And_Pictographs_Ext_A[1] = {0x1FA70,0x1FAFF};
inline constexpr yuki::CInterval<char32_t> Symbols_For_Legacy_Computing[1] = {0x1FB00,0x1FBFF};
inline constexpr yuki::CInterval<char32_t> CJK_Ext_B[1] = {0x20000,0x2A6DF};
inline constexpr yuki::CInterval<char32_t> CJK_Ext_C[1] = {0x2A700,0x2B73F};
inline constexpr yuki::CInterval<char32_t> CJK_Ext_D[1] = {0x2B740,0x2B81F};
inline constexpr yuki::CInterval<char32_t> CJK_Ext_E[1] = {0x2B820,0x2CEAF};
inline constexpr yuki::CInterval<char32_t> CJK_Ext_F[1] = {0x2CEB0,0x2EBEF};
inline constexpr yuki::CInterval<char32_t> CJK_Ext_I[1] = {0x2EBF0,0x2EE5F};
inline constexpr yuki::CInterval<char32_t> CJK_Compat_Ideographs_Sup[1] = {0x2F800,0x2FA1F};
inline constexpr yuki::CInterval<char32_t> CJK_Ext_G[1] = {0x30000,0x3134F};
inline constexpr yuki::CInterval<char32_t> CJK_Ext_H[1] = {0x31350,0x323AF};
inline constexpr yuki::CInterval<char32_t> Tags[1] = {0xE0000,0xE007F};
inline constexpr yuki::CInterval<char32_t> VS_Sup[1] = {0xE0100,0xE01EF};
inline constexpr yuki::CInterval<char32_t> Sup_PUA_A[1] = {0xF0000,0xFFFFF};
inline constexpr yuki::CInterval<char32_t> Sup_PUA_B[1] = {0x100000,0x10FFFF};
inline constexpr yuki::CInterval<char32_t> NB[51]={
    {0x2FE0,0x2FEF},{0x10200,0x1027F},{0x103E0,0x103FF},{0x105C0,0x105FF},{0x107C0,0x107FF},{0x108B0,0x108DF},{0x10940,0x1097F},{0x10AA0,0x10ABF},
    {0x10BB0,0x10BFF},{0x10C50,0x10C7F},{0x10D40,0x10E5F},{0x11250,0x1127F},{0x11380,0x113FF},{0x114E0,0x1157F},{0x116D0,0x116FF},{0x11750,0x117FF},
    {0x11850,0x1189F},{0x11960,0x1199F},{0x11B60,0x11BFF},{0x11CC0,0x11CFF},{0x11DB0,0x11EDF},{0x11F60,0x11FAF},{0x12550,0x12F8F},{0x13460,0x143FF},
    {0x14680,0x167FF},{0x16B90,0x16E3F},{0x16EA0,0x16EFF},{0x16FA0,0x16FDF},{0x18D80,0x1AFEF},{0x1B300,0x1BBFF},{0x1BCB0,0x1CEFF},{0x1CFD0,0x1CFFF},
    {0x1D250,0x1D2BF},{0x1D380,0x1D3FF},{0x1DAB0,0x1DEFF},{0x1E090,0x1E0FF},{0x1E150,0x1E28F},{0x1E300,0x1E4CF},{0x1E500,0x1E7DF},{0x1E8E0,0x1E8FF},
    {0x1E960,0x1EC6F},{0x1ECC0,0x1ECFF},{0x1ED50,0x1EDFF},{0x1EF00,0x1EFFF},{0x1FC00,0x1FFFF},{0x2A6E0,0x2A6FF},{0x2EE60,0x2F7FF},{0x2FA20,0x2FFFF},
    {0x323B0,0xDFFFF},{0xE0080,0xE00FF},{0xE01F0,0xEFFFF},
};
inline constexpr size_t NB_total=820320;


enum struct Block : unsigned short {
    NB,Adlam, Aegean_Numbers, Ahom, Alchemical, Alphabetic_PF, Anatolian_Hieroglyphs, Ancient_Greek_Music,
    Ancient_Greek_Numbers, Ancient_Symbols, Arabic, Arabic_Ext_A, Arabic_Ext_B, Arabic_Ext_C, Arabic_Math, Arabic_PF_A,
    Arabic_PF_B, Arabic_Sup, Armenian, Arrows, ASCII, Avestan, Balinese, Bamum,
    Bamum_Sup, Bassa_Vah, Batak, Bengali, Bhaiksuki, Block_Elements, Bopomofo, Bopomofo_Ext,
    Box_Drawing, Brahmi, Braille, Buginese, Buhid, Byzantine_Music, Carian, Caucasian_Albanian,
    Chakma, Cham, Cherokee, Cherokee_Sup, Chess_Symbols, Chorasmian, CJK, CJK_Compat,
    CJK_Compat_Forms, CJK_Compat_Ideographs, CJK_Compat_Ideographs_Sup, CJK_Ext_A, CJK_Ext_B, CJK_Ext_C, CJK_Ext_D, CJK_Ext_E,
    CJK_Ext_F, CJK_Ext_G, CJK_Ext_H, CJK_Ext_I, CJK_Radicals_Sup, CJK_Strokes, CJK_Symbols, Compat_Jamo,
    Control_Pictures, Coptic, Coptic_Epact_Numbers, Counting_Rod, Cuneiform, Cuneiform_Numbers, Currency_Symbols, Cypriot_Syllabary,
    Cypro_Minoan, Cyrillic, Cyrillic_Ext_A, Cyrillic_Ext_B, Cyrillic_Ext_C, Cyrillic_Ext_D, Cyrillic_Sup, Deseret,
    Devanagari, Devanagari_Ext, Devanagari_Ext_A, Diacriticals, Diacriticals_Ext, Diacriticals_For_Symbols, Diacriticals_Sup, Dingbats,
    Dives_Akuru, Dogra, Domino, Duployan, Early_Dynastic_Cuneiform, Egyptian_Hieroglyph_Format_Controls, Egyptian_Hieroglyphs, Elbasan,
    Elymaic, Emoticons, Enclosed_Alphanum, Enclosed_Alphanum_Sup, Enclosed_CJK, Enclosed_Ideographic_Sup, Ethiopic, Ethiopic_Ext,
    Ethiopic_Ext_A, Ethiopic_Ext_B, Ethiopic_Sup, Geometric_Shapes, Geometric_Shapes_Ext, Georgian, Georgian_Ext, Georgian_Sup,
    Glagolitic, Glagolitic_Sup, Gothic, Grantha, Greek, Greek_Ext, Gujarati, Gunjala_Gondi,
    Gurmukhi, Half_And_Full_Forms, Half_Marks, Hangul, Hanifi_Rohingya, Hanunoo, Hatran, Hebrew,
    High_PU_Surrogates, High_Surrogates, Hiragana, IDC, Ideographic_Symbols, Imperial_Aramaic, Indic_Number_Forms, Indic_Siyaq_Numbers,
    Inscriptional_Pahlavi, Inscriptional_Parthian, IPA_Ext, Jamo, Jamo_Ext_A, Jamo_Ext_B, Javanese, Kaithi,
    Kaktovik_Numerals, Kana_Ext_A, Kana_Ext_B, Kana_Sup, Kanbun, Kangxi, Kannada, Katakana,
    Katakana_Ext, Kawi, Kayah_Li, Kharoshthi, Khitan_Small_Script, Khmer, Khmer_Symbols, Khojki,
    Khudawadi, Lao, Latin_1_Sup, Latin_Ext_A, Latin_Ext_Additional, Latin_Ext_B, Latin_Ext_C, Latin_Ext_D,
    Latin_Ext_E, Latin_Ext_F, Latin_Ext_G, Lepcha, Letterlike_Symbols, Limbu, Linear_A, Linear_B_Ideograms,
    Linear_B_Syllabary, Lisu, Lisu_Sup, Low_Surrogates, Lycian, Lydian, Mahajani, Mahjong,
    Makasar, Malayalam, Mandaic, Manichaean, Marchen, Masaram_Gondi, Math_Alphanum, Math_Operators,
    Mayan_Numerals, Medefaidrin, Meetei_Mayek, Meetei_Mayek_Ext, Mende_Kikakui, Meroitic_Cursive, Meroitic_Hieroglyphs, Miao,
    Misc_Arrows, Misc_Math_Symbols_A, Misc_Math_Symbols_B, Misc_Pictographs, Misc_Symbols, Misc_Technical, Modi, Modifier_Letters,
    Modifier_Tone_Letters, Mongolian, Mongolian_Sup, Mro, Multani, Music, Myanmar, Myanmar_Ext_A,
    Myanmar_Ext_B, Nabataean, Nag_Mundari, Nandinagari, New_Tai_Lue, Newa, NKo, Number_Forms,
    Nushu, Nyiakeng_Puachue_Hmong, OCR, Ogham, Ol_Chiki, Old_Hungarian, Old_Italic, Old_North_Arabian,
    Old_Permic, Old_Persian, Old_Sogdian, Old_South_Arabian, Old_Turkic, Old_Uyghur, Oriya, Ornamental_Dingbats,
    Osage, Osmanya, Ottoman_Siyaq_Numbers, Pahawh_Hmong, Palmyrene, Pau_Cin_Hau, Phags_Pa, Phaistos,
    Phoenician, Phonetic_Ext, Phonetic_Ext_Sup, Playing_Cards, Psalter_Pahlavi, PUA, Punctuation, Rejang,
    Rumi, Runic, Samaritan, Saurashtra, Sharada, Shavian, Shorthand_Format_Controls, Siddham,
    Sinhala, Sinhala_Archaic_Numbers, Small_Forms, Small_Kana_Ext, Sogdian, Sora_Sompeng, Soyombo, Specials,
    Sundanese, Sundanese_Sup, Sup_Arrows_A, Sup_Arrows_B, Sup_Arrows_C, Sup_Math_Operators, Sup_PUA_A, Sup_PUA_B,
    Sup_Punctuation, Sup_Symbols_And_Pictographs, Super_And_Sub, Sutton_SignWriting, Syloti_Nagri, Symbols_And_Pictographs_Ext_A, Symbols_For_Legacy_Computing, Syriac,
    Syriac_Sup, Tagalog, Tagbanwa, Tags, Tai_Le, Tai_Tham, Tai_Viet, Tai_Xuan_Jing,
    Takri, Tamil, Tamil_Sup, Tangsa, Tangut, Tangut_Components, Tangut_Sup, Telugu,
    Thaana, Thai, Tibetan, Tifinagh, Tirhuta, Toto, Transport_And_Map, UCAS,
    UCAS_Ext, UCAS_Ext_A, Ugaritic, Vai, Vedic_Ext, Vertical_Forms, Vithkuqi, VS,
    VS_Sup, Wancho, Warang_Citi, Yezidi, Yi_Radicals, Yi_Syllables, Yijing, Zanabazar_Square,
    Znamenny_Music, total_
}; // enum struct Block


inline constexpr Name_CC_Num<Block> block_table[329]={
    {"NB",NB,0},{"Adlam",Adlam,1},{"Aegean_Numbers",Aegean_Numbers,2},{"Ahom_blk",Ahom_blk,3},
    {"Alchemical",Alchemical,4},{"Alphabetic_PF",Alphabetic_PF,5},{"Anatolian_Hieroglyphs",Anatolian_Hieroglyphs,6},{"Ancient_Greek_Music",Ancient_Greek_Music,7},
    {"Ancient_Greek_Numbers",Ancient_Greek_Numbers,8},{"Ancient_Symbols",Ancient_Symbols,9},{"Arabic",Arabic,10},{"Arabic_Ext_A",Arabic_Ext_A,11},
    {"Arabic_Ext_B",Arabic_Ext_B,12},{"Arabic_Ext_C",Arabic_Ext_C,13},{"Arabic_Math",Arabic_Math,14},{"Arabic_PF_A",Arabic_PF_A,15},
    {"Arabic_PF_B",Arabic_PF_B,16},{"Arabic_Sup",Arabic_Sup,17},{"Armenian",Armenian,18},{"Arrows",Arrows,19},
    {"ASCII",ASCII,20},{"Avestan",Avestan,21},{"Balinese",Balinese,22},{"Bamum",Bamum,23},
    {"Bamum_Sup",Bamum_Sup,24},{"Bassa_Vah",Bassa_Vah,25},{"Batak",Batak,26},{"Bengali",Bengali,27},
    {"Bhaiksuki",Bhaiksuki,28},{"Block_Elements",Block_Elements,29},{"Bopomofo",Bopomofo,30},{"Bopomofo_Ext",Bopomofo_Ext,31},
    {"Box_Drawing",Box_Drawing,32},{"Brahmi",Brahmi,33},{"Braille",Braille,34},{"Buginese",Buginese,35},
    {"Buhid",Buhid,36},{"Byzantine_Music",Byzantine_Music,37},{"Carian",Carian,38},{"Caucasian_Albanian",Caucasian_Albanian,39},
    {"Chakma",Chakma,40},{"Cham_blk",Cham_blk,41},{"Cherokee",Cherokee,42},{"Cherokee_Sup",Cherokee_Sup,43},
    {"Chess_Symbols",Chess_Symbols,44},{"Chorasmian",Chorasmian,45},{"CJK",CJK,46},{"CJK_Compat",CJK_Compat,47},
    {"CJK_Compat_Forms",CJK_Compat_Forms,48},{"CJK_Compat_Ideographs",CJK_Compat_Ideographs,49},{"CJK_Compat_Ideographs_Sup",CJK_Compat_Ideographs_Sup,50},{"CJK_Ext_A",CJK_Ext_A,51},
    {"CJK_Ext_B",CJK_Ext_B,52},{"CJK_Ext_C",CJK_Ext_C,53},{"CJK_Ext_D",CJK_Ext_D,54},{"CJK_Ext_E",CJK_Ext_E,55},
    {"CJK_Ext_F",CJK_Ext_F,56},{"CJK_Ext_G",CJK_Ext_G,57},{"CJK_Ext_H",CJK_Ext_H,58},{"CJK_Ext_I",CJK_Ext_I,59},
    {"CJK_Radicals_Sup",CJK_Radicals_Sup,60},{"CJK_Strokes",CJK_Strokes,61},{"CJK_Symbols",CJK_Symbols,62},{"Compat_Jamo",Compat_Jamo,63},
    {"Control_Pictures",Control_Pictures,64},{"Coptic",Coptic,65},{"Coptic_Epact_Numbers",Coptic_Epact_Numbers,66},{"Counting_Rod",Counting_Rod,67},
    {"Cuneiform",Cuneiform,68},{"Cuneiform_Numbers",Cuneiform_Numbers,69},{"Currency_Symbols",Currency_Symbols,70},{"Cypriot_Syllabary",Cypriot_Syllabary,71},
    {"Cypro_Minoan",Cypro_Minoan,72},{"Cyrillic",Cyrillic,73},{"Cyrillic_Ext_A",Cyrillic_Ext_A,74},{"Cyrillic_Ext_B",Cyrillic_Ext_B,75},
    {"Cyrillic_Ext_C",Cyrillic_Ext_C,76},{"Cyrillic_Ext_D",Cyrillic_Ext_D,77},{"Cyrillic_Sup",Cyrillic_Sup,78},{"Deseret",Deseret,79},
    {"Devanagari",Devanagari,80},{"Devanagari_Ext",Devanagari_Ext,81},{"Devanagari_Ext_A",Devanagari_Ext_A,82},{"Diacriticals",Diacriticals,83},
    {"Diacriticals_Ext",Diacriticals_Ext,84},{"Diacriticals_For_Symbols",Diacriticals_For_Symbols,85},{"Diacriticals_Sup",Diacriticals_Sup,86},{"Dingbats",Dingbats,87},
    {"Dives_Akuru",Dives_Akuru,88},{"Dogra",Dogra,89},{"Domino",Domino,90},{"Duployan",Duployan,91},
    {"Early_Dynastic_Cuneiform",Early_Dynastic_Cuneiform,92},{"Egyptian_Hieroglyph_Format_Controls",Egyptian_Hieroglyph_Format_Controls,93},{"Egyptian_Hieroglyphs",Egyptian_Hieroglyphs,94},{"Elbasan",Elbasan,95},
    {"Elymaic",Elymaic,96},{"Emoticons",Emoticons,97},{"Enclosed_Alphanum",Enclosed_Alphanum,98},{"Enclosed_Alphanum_Sup",Enclosed_Alphanum_Sup,99},
    {"Enclosed_CJK",Enclosed_CJK,100},{"Enclosed_Ideographic_Sup",Enclosed_Ideographic_Sup,101},{"Ethiopic",Ethiopic,102},{"Ethiopic_Ext",Ethiopic_Ext,103},
    {"Ethiopic_Ext_A",Ethiopic_Ext_A,104},{"Ethiopic_Ext_B",Ethiopic_Ext_B,105},{"Ethiopic_Sup",Ethiopic_Sup,106},{"Geometric_Shapes",Geometric_Shapes,107},
    {"Geometric_Shapes_Ext",Geometric_Shapes_Ext,108},{"Georgian",Georgian,109},{"Georgian_Ext",Georgian_Ext,110},{"Georgian_Sup",Georgian_Sup,111},
    {"Glagolitic",Glagolitic,112},{"Glagolitic_Sup",Glagolitic_Sup,113},{"Gothic",Gothic,114},{"Grantha",Grantha,115},
    {"Greek",Greek,116},{"Greek_Ext",Greek_Ext,117},{"Gujarati",Gujarati,118},{"Gunjala_Gondi",Gunjala_Gondi,119},
    {"Gurmukhi",Gurmukhi,120},{"Half_And_Full_Forms",Half_And_Full_Forms,121},{"Half_Marks",Half_Marks,122},{"Hangul",Hangul,123},
    {"Hanifi_Rohingya",Hanifi_Rohingya,124},{"Hanunoo",Hanunoo,125},{"Hatran",Hatran,126},{"Hebrew",Hebrew,127},
    {"High_PU_Surrogates",High_PU_Surrogates,128},{"High_Surrogates",High_Surrogates,129},{"Hiragana",Hiragana,130},{"IDC_blk",IDC_blk,131},
    {"Ideographic_Symbols",Ideographic_Symbols,132},{"Imperial_Aramaic",Imperial_Aramaic,133},{"Indic_Number_Forms",Indic_Number_Forms,134},{"Indic_Siyaq_Numbers",Indic_Siyaq_Numbers,135},
    {"Inscriptional_Pahlavi",Inscriptional_Pahlavi,136},{"Inscriptional_Parthian",Inscriptional_Parthian,137},{"IPA_Ext",IPA_Ext,138},{"Jamo",Jamo,139},
    {"Jamo_Ext_A",Jamo_Ext_A,140},{"Jamo_Ext_B",Jamo_Ext_B,141},{"Javanese",Javanese,142},{"Kaithi",Kaithi,143},
    {"Kaktovik_Numerals",Kaktovik_Numerals,144},{"Kana_Ext_A",Kana_Ext_A,145},{"Kana_Ext_B",Kana_Ext_B,146},{"Kana_Sup",Kana_Sup,147},
    {"Kanbun",Kanbun,148},{"Kangxi",Kangxi,149},{"Kannada",Kannada,150},{"Katakana",Katakana,151},
    {"Katakana_Ext",Katakana_Ext,152},{"Kawi_blk",Kawi_blk,153},{"Kayah_Li",Kayah_Li,154},{"Kharoshthi",Kharoshthi,155},
    {"Khitan_Small_Script",Khitan_Small_Script,156},{"Khmer",Khmer,157},{"Khmer_Symbols",Khmer_Symbols,158},{"Khojki",Khojki,159},
    {"Khudawadi",Khudawadi,160},{"Lao",Lao,161},{"Latin_1_Sup",Latin_1_Sup,162},{"Latin_Ext_A",Latin_Ext_A,163},
    {"Latin_Ext_Additional",Latin_Ext_Additional,164},{"Latin_Ext_B",Latin_Ext_B,165},{"Latin_Ext_C",Latin_Ext_C,166},{"Latin_Ext_D",Latin_Ext_D,167},
    {"Latin_Ext_E",Latin_Ext_E,168},{"Latin_Ext_F",Latin_Ext_F,169},{"Latin_Ext_G",Latin_Ext_G,170},{"Lepcha",Lepcha,171},
    {"Letterlike_Symbols",Letterlike_Symbols,172},{"Limbu",Limbu,173},{"Linear_A",Linear_A,174},{"Linear_B_Ideograms",Linear_B_Ideograms,175},
    {"Linear_B_Syllabary",Linear_B_Syllabary,176},{"Lisu_blk",Lisu_blk,177},{"Lisu_Sup",Lisu_Sup,178},{"Low_Surrogates",Low_Surrogates,179},
    {"Lycian",Lycian,180},{"Lydian",Lydian,181},{"Mahajani",Mahajani,182},{"Mahjong",Mahjong,183},
    {"Makasar",Makasar,184},{"Malayalam",Malayalam,185},{"Mandaic",Mandaic,186},{"Manichaean",Manichaean,187},
    {"Marchen",Marchen,188},{"Masaram_Gondi",Masaram_Gondi,189},{"Math_Alphanum",Math_Alphanum,190},{"Math_Operators",Math_Operators,191},
    {"Mayan_Numerals",Mayan_Numerals,192},{"Medefaidrin",Medefaidrin,193},{"Meetei_Mayek",Meetei_Mayek,194},{"Meetei_Mayek_Ext",Meetei_Mayek_Ext,195},
    {"Mende_Kikakui",Mende_Kikakui,196},{"Meroitic_Cursive",Meroitic_Cursive,197},{"Meroitic_Hieroglyphs",Meroitic_Hieroglyphs,198},{"Miao",Miao,199},
    {"Misc_Arrows",Misc_Arrows,200},{"Misc_Math_Symbols_A",Misc_Math_Symbols_A,201},{"Misc_Math_Symbols_B",Misc_Math_Symbols_B,202},{"Misc_Pictographs",Misc_Pictographs,203},
    {"Misc_Symbols",Misc_Symbols,204},{"Misc_Technical",Misc_Technical,205},{"Modi_blk",Modi_blk,206},{"Modifier_Letters",Modifier_Letters,207},
    {"Modifier_Tone_Letters",Modifier_Tone_Letters,208},{"Mongolian",Mongolian,209},{"Mongolian_Sup",Mongolian_Sup,210},{"Mro",Mro,211},
    {"Multani",Multani,212},{"Music",Music,213},{"Myanmar",Myanmar,214},{"Myanmar_Ext_A",Myanmar_Ext_A,215},
    {"Myanmar_Ext_B",Myanmar_Ext_B,216},{"Nabataean",Nabataean,217},{"Nag_Mundari",Nag_Mundari,218},{"Nandinagari",Nandinagari,219},
    {"New_Tai_Lue",New_Tai_Lue,220},{"Newa_blk",Newa_blk,221},{"NKo",NKo,222},{"Number_Forms",Number_Forms,223},
    {"Nushu",Nushu,224},{"Nyiakeng_Puachue_Hmong",Nyiakeng_Puachue_Hmong,225},{"OCR",OCR,226},{"Ogham",Ogham,227},
    {"Ol_Chiki",Ol_Chiki,228},{"Old_Hungarian",Old_Hungarian,229},{"Old_Italic",Old_Italic,230},{"Old_North_Arabian",Old_North_Arabian,231},
    {"Old_Permic",Old_Permic,232},{"Old_Persian",Old_Persian,233},{"Old_Sogdian",Old_Sogdian,234},{"Old_South_Arabian",Old_South_Arabian,235},
    {"Old_Turkic",Old_Turkic,236},{"Old_Uyghur",Old_Uyghur,237},{"Oriya",Oriya,238},{"Ornamental_Dingbats",Ornamental_Dingbats,239},
    {"Osage",Osage,240},{"Osmanya",Osmanya,241},{"Ottoman_Siyaq_Numbers",Ottoman_Siyaq_Numbers,242},{"Pahawh_Hmong",Pahawh_Hmong,243},
    {"Palmyrene",Palmyrene,244},{"Pau_Cin_Hau",Pau_Cin_Hau,245},{"Phags_Pa",Phags_Pa,246},{"Phaistos",Phaistos,247},
    {"Phoenician",Phoenician,248},{"Phonetic_Ext",Phonetic_Ext,249},{"Phonetic_Ext_Sup",Phonetic_Ext_Sup,250},{"Playing_Cards",Playing_Cards,251},
    {"Psalter_Pahlavi",Psalter_Pahlavi,252},{"PUA",PUA,253},{"Punctuation",Punctuation,254},{"Rejang",Rejang,255},
    {"Rumi",Rumi,256},{"Runic",Runic,257},{"Samaritan",Samaritan,258},{"Saurashtra",Saurashtra,259},
    {"Sharada",Sharada,260},{"Shavian",Shavian,261},{"Shorthand_Format_Controls",Shorthand_Format_Controls,262},{"Siddham",Siddham,263},
    {"Sinhala",Sinhala,264},{"Sinhala_Archaic_Numbers",Sinhala_Archaic_Numbers,265},{"Small_Forms",Small_Forms,266},{"Small_Kana_Ext",Small_Kana_Ext,267},
    {"Sogdian",Sogdian,268},{"Sora_Sompeng",Sora_Sompeng,269},{"Soyombo",Soyombo,270},{"Specials",Specials,271},
    {"Sundanese",Sundanese,272},{"Sundanese_Sup",Sundanese_Sup,273},{"Sup_Arrows_A",Sup_Arrows_A,274},{"Sup_Arrows_B",Sup_Arrows_B,275},
    {"Sup_Arrows_C",Sup_Arrows_C,276},{"Sup_Math_Operators",Sup_Math_Operators,277},{"Sup_PUA_A",Sup_PUA_A,278},{"Sup_PUA_B",Sup_PUA_B,279},
    {"Sup_Punctuation",Sup_Punctuation,280},{"Sup_Symbols_And_Pictographs",Sup_Symbols_And_Pictographs,281},{"Super_And_Sub",Super_And_Sub,282},{"Sutton_SignWriting",Sutton_SignWriting,283},
    {"Syloti_Nagri",Syloti_Nagri,284},{"Symbols_And_Pictographs_Ext_A",Symbols_And_Pictographs_Ext_A,285},{"Symbols_For_Legacy_Computing",Symbols_For_Legacy_Computing,286},{"Syriac",Syriac,287},
    {"Syriac_Sup",Syriac_Sup,288},{"Tagalog",Tagalog,289},{"Tagbanwa",Tagbanwa,290},{"Tags",Tags,291},
    {"Tai_Le",Tai_Le,292},{"Tai_Tham",Tai_Tham,293},{"Tai_Viet",Tai_Viet,294},{"Tai_Xuan_Jing",Tai_Xuan_Jing,295},
    {"Takri",Takri,296},{"Tamil",Tamil,297},{"Tamil_Sup",Tamil_Sup,298},{"Tangsa",Tangsa,299},
    {"Tangut",Tangut,300},{"Tangut_Components",Tangut_Components,301},{"Tangut_Sup",Tangut_Sup,302},{"Telugu",Telugu,303},
    {"Thaana",Thaana,304},{"Thai_blk",Thai_blk,305},{"Tibetan",Tibetan,306},{"Tifinagh",Tifinagh,307},
    {"Tirhuta",Tirhuta,308},{"Toto_blk",Toto_blk,309},{"Transport_And_Map",Transport_And_Map,310},{"UCAS",UCAS,311},
    {"UCAS_Ext",UCAS_Ext,312},{"UCAS_Ext_A",UCAS_Ext_A,313},{"Ugaritic",Ugaritic,314},{"Vai",Vai,315},
    {"Vedic_Ext",Vedic_Ext,316},{"Vertical_Forms",Vertical_Forms,317},{"Vithkuqi",Vithkuqi,318},{"VS_blk",VS_blk,319},
    {"VS_Sup",VS_Sup,320},{"Wancho",Wancho,321},{"Warang_Citi",Warang_Citi,322},{"Yezidi",Yezidi,323},
    {"Yi_Radicals",Yi_Radicals,324},{"Yi_Syllables",Yi_Syllables,325},{"Yijing",Yijing,326},{"Zanabazar_Square",Zanabazar_Square,327},
    {"Znamenny_Music",Znamenny_Music,328},
};


constexpr Block block(const char32_t c){
    const Name_CC_Num<Block>* const p = yuki::find_in_cintervals(
        block_table+1,
        block_table+sizeof(block_table)/sizeof(Name_CC_Num<Block>),
        c,
        [](const Name_CC_Num<Block> nccn){return *nccn.cc;},
        yuki::Less<yuki::CInterval<char32_t>>{}
    );
    return p!=block_table+sizeof(block_table)/sizeof(Name_CC_Num<Block>) ? p->num : Block::NB;
}
} // namespace yuki::unicode
