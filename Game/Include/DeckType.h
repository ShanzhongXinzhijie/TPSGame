#pragma once

static constexpr int DECK_TYPE_MAX = 12;
static constexpr wchar_t DECK_TYPE[DECK_TYPE_MAX][20] = {
	L"",
	L"アグロ",
	L"ミッドレンジ",
	L"コントロール",
	L"ランプ",
	L"アーティファクト",
	L"ふくしゅう",
	L"にんぎょう",
	L"ニュートラル",
	L"OTK",
	L"ひじゅつ",
	L"じしょう",
};

static constexpr int CHARA_TYPE_MAX = 8;
static constexpr wchar_t CHARA_TYPE[CHARA_TYPE_MAX][20] = {
	//L""
	L"エルフ",
	L"ロイヤル",
	L"ウィッチ",
	L"ドラゴン",
	L"ネクロ",
	L"ヴァンパイア",
	L"ビショップ",
	L"ネメシス",
};