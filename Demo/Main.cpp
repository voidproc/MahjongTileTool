# include <Siv3D.hpp> // Siv3D v0.6.16

struct TileInfo
{
	Texture faceTexture;
};

const Array<String> TileNameList = {
	U"character_1",
	U"character_2",
	U"character_3",
	U"character_4",
	U"character_5",
	U"character_6",
	U"character_7",
	U"character_8",
	U"character_9",
	U"circle_1",
	U"circle_2",
	U"circle_3",
	U"circle_4",
	U"circle_5",
	U"circle_6",
	U"circle_7",
	U"circle_8",
	U"circle_9",
	U"bamboo_1",
	U"bamboo_2",
	U"bamboo_3",
	U"bamboo_4",
	U"bamboo_5",
	U"bamboo_6",
	U"bamboo_7",
	U"bamboo_8",
	U"bamboo_9",
	U"wind_east",
	U"wind_south",
	U"wind_west",
	U"wind_north",
	U"dragon_white",
	U"dragon_green",
	U"dragon_red",
};

constexpr std::array<Color, 3> TileLayerColors = {
	ColorF{ 0.2, 0.4, 1 }.lerp(Palette::Black, 0.6),
	Palette::Green.lerp(Palette::Black, 0.3),
	Palette::Darkred
};

constexpr Size TileFaceImageSize{ 256, 373 };

void DrawTileAt(const Vec2& pos, const SizeF& tileSize, const TileInfo& tileInfo)
{
	const Color TileFaceColor = Palette::White.lerp(Palette::Wheat, 0.1);
	const Color TileBorderColor = Palette::Black;
	const double TileBorderWidth = 4.0;
	const double TileBorderRadius = 8.0;
	const double TileFaceScale = 0.90;

	const auto scaledTexture = tileInfo.faceTexture.resized(tileSize);
	scaledTexture.regionAt(pos).stretched(0, 0, 24, 0).rounded(TileBorderRadius).draw(Palette::Orange).drawFrame(TileBorderWidth, TileBorderColor);
	scaledTexture.regionAt(pos).stretched(0, 0, 12, 0).rounded(TileBorderRadius).draw(Palette::Wheat.lerp(Palette::Gray, 0.3));
	scaledTexture.regionAt(pos).rounded(TileBorderRadius).draw(TileFaceColor).drawFrame(TileBorderWidth, Palette::Wheat);
	scaledTexture.regionAt(pos).stretched(0, 0, 24, 0).rounded(TileBorderRadius).drawFrame(TileBorderWidth, TileBorderColor);
	scaledTexture.scaled(TileFaceScale).drawAt(pos);
}

void Main()
{
	Scene::Resize(1920, 1080);
	Scene::SetResizeMode(ResizeMode::Keep);
	Window::Resize((Scene::Size() * 0.75).asPoint());

	Scene::SetBackground(ColorF{ 0, 0.4, 0.25 });

	Array<TileInfo> tileInfoList;

	for (const auto name : TileNameList)
	{
		Image image{ TileFaceImageSize, ColorF{ 0, 0 } };

		for (int iLayer = 0; iLayer < 3; ++iLayer)
		{
			const FilePath path = U"image/tiles_{}/{}.png"_fmt(iLayer + 1, name);

			if (not FileSystem::Exists(path)) continue;

			Image{ path }.negated().stamp(image, 0, 0, TileLayerColors[iLayer]);
		}

		tileInfoList << TileInfo{ .faceTexture = Texture{ image } };
	}

	while (System::Update())
	{
		{
			const int columns = 9;
			const int rows = tileInfoList.size() / columns + 1;
			const double tileWidth = 128;
			const SizeF tileSize{ tileWidth, TileFaceImageSize.y * tileWidth / TileFaceImageSize.x };
			const SizeF margin{ 16, 48 };

			const SizeF drawAreaSize{ tileSize.x * columns + margin.x * (columns - 1), tileSize.y * rows + margin.y * (rows - 1) + margin.y / 2 };
			Transformer2D translate{ Mat3x2::Translate((Scene::Size() - drawAreaSize) / 2) };

			for (const auto [index, tileInfo] : Indexed(tileInfoList))
			{
				const Vec2 pos{ (index % columns) * (tileSize.x + margin.x), (index / columns) * (tileSize.y + margin.y) };
				DrawTileAt(pos + tileSize / 2, tileSize, tileInfo);
			}
		}
	}
}
