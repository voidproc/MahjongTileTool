# include <Siv3D.hpp> // Siv3D v0.6.16

/*
 * 麻雀牌画像を保存する
 *
 * 使用フォント: GL-MahjongTile (https://github.com/Gutenberg-Labo/DingbatFonts)
 */

namespace
{
	constexpr Size SceneSize{ 1600, 1200 };
	constexpr Size WindowSize{ 1600, 1200 };

	// 出力する画像の横幅を固定
	constexpr int32 ImageWidth = 256;

	// 縦横比（適当）から画像の高さを決める
	constexpr Size ImageSize{ ImageWidth, static_cast<int32>(ImageWidth * 1.46) };

	// 枠を描画したくないので、枠が上手く見切れるようにフォントサイズを設定
	constexpr int32 FontSize = static_cast<int32>(ImageWidth * 1.97);
}

void Main()
{
	Scene::SetBackground(Palette::White);

	// テスト描画用シーンとウィンドウ
	Scene::Resize(SceneSize);
	Scene::SetResizeMode(ResizeMode::Keep);
	Window::Resize(WindowSize);

	const Font fontMahjong{ FontSize, U"font/GL-MahjongTile.ttf" };

	const Array<std::pair<String, String>> tilesAll = {
		{ U"1234", U"wind" },
		{ U"567", U"dragon" },
		{ U"qwertyuio", U"character" },
		{ U"asdfghjkl", U"bamboo" },
		{ U"zxcvbnm,.", U"circle" },
	};

	Array<Texture> textures;

	// 画像を保存
	for (const auto& tiles : tilesAll)
	{
		for (const auto [index, tile] : Indexed(tiles.first))
		{
			constexpr Size TempImageSize{ FontSize, FontSize };
			Image image = Image{ TempImageSize, ColorF{ 0.0, 0.0 } }.clipped((TempImageSize - ImageSize) / 2, ImageSize);

			fontMahjong(tile)
				.stampAt(image, image.size() / 2, Palette::Black);

			textures << Texture{ image };

			const FilePath path = U"output/{}_{}.png"_fmt(tiles.second, index + 1);
			image.savePNG(path);
		}
	}

	while (System::Update())
	{
		// 切り出す領域を描画
		{
			Transformer2D translate{ Mat3x2::Translate(64, 64).scaled(0.5) };
			const auto region = fontMahjong(U"1").draw(0, 0, Palette::Black);
			RectF{ Arg::center = region.center(), ImageSize }.drawFrame(1.0, Palette::Magenta);
		}

		// 出力した画像を描画
		{
			Transformer2D translate{ Mat3x2::Translate(480, 64).scaled(0.5) };

			for (auto [index, texture] : Indexed(textures))
			{
				const int cols = 8;
				const int x = index % cols;
				const int y = index / cols;

				const auto region = texture
					.draw(x * (ImageSize.x + 16), y * (ImageSize.y + 16), ColorF{ 1.0 });

				region.drawFrame(4.0, Palette::Black);
			}
		}
	}
}
