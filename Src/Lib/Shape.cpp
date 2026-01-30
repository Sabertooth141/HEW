// /**
// * @file Shape.cpp/h
// * @brief 2D図形描画関数群（実装）
// * @revison 1
// *
// * 2025 HAL Osaka. All rights reserved.
// */
// #include "Shape.h"
// using namespace std;
//
// /**
// * 線を描く関数
// * @param _x1	始点X座標
// * @param _y1	始点Y座標
// * @param _x2	終点X座標
// * @param _y2	終点Y座標
// * @param _cc	色コード
// */
// void DrawLine(int _x1, int _y1, int _x2, int _y2, int _cc)
// {
// 	// ▼DDAアルゴリズムで線を引く
// 	// DX、DY、ステップ数計算
// 	int dx = _x2 - _x1;
// 	int dy = _y2 - _y1;
// 	// ステップ数はX方向とY方向の大きい方を使う
// 	int steps = abs(dx) > abs(dy) ? abs(dx) : abs(dy);
// 	// X/Y方向の増分を浮動小数点で求める
// 	float x_inc = dx / (float)steps;
// 	float y_inc = dy / (float)steps;
// 	// プロットする点も浮動小数点で保持
// 	float x = _x1;	// 開始点X座標
// 	float y = _y1;	// 開始点Y座標
// 	// ピクセル描画ループ
// 	for (int i = 0; i <= steps; i++) {
// 		// 四捨五入してピクセル描画
// 		DrawPixel((int)(x + 0.5f), (int)(y + 0.5f), _cc);
// 		// 次の点へ
// 		x += x_inc;
// 		y += y_inc;
// 	}
// }	// DrawLine
//
// /**
// * 四角形を描く関数
// * @param _x1	左上X座標
// * @param _y1	左上Y座標
// * @param _x2	右下X座標
// * @param _y2	右下Y座標
// * @param _cc	色コード
// * @param _fill	塗りつぶしフラグ（true: 塗りつぶし、false: 枠線のみ）
// */
// void DrawRect(int _x1, int _y1, int _x2, int _y2, int _cc, bool _fill)
// {
// 	// 塗りつぶし
// 	if (_fill) {
// 		// スキャンライン塗りつぶし
// 		for (int yy = _y1; yy <= _y2; yy++) {
// 			DrawLine(_x1, yy, _x2, yy, _cc);
// 		}
// 		return;
// 	}
// 	// 枠線（非塗りつぶし）
// 	DrawLine(_x1, _y1, _x2, _y1, _cc);	// 上辺
// 	DrawLine(_x1, _y1 + 1, _x1, _y2 - 1, _cc);	// 左辺
// 	DrawLine(_x2, _y1 + 1, _x2, _y2 - 1, _cc);	// 右辺
// 	DrawLine(_x1, _y2, _x2, _y2, _cc);	// 下辺
// }	// DrawRect
//
// /**
// * 円を描く関数
// * @param _cx	中心X座標
// * @param _cy	中心Y座標
// * @param _r	半径
// * @param _cc	色コード
// * @param _fill	塗りつぶしフラグ（true: 塗りつぶし、false: 枠線のみ）
// */
// void DrawCircle(int _cx, int _cy, int _r, int _cc, bool _fill)
// {
// 	// 塗りつぶし
// 	if (_fill) {
// 		if (_r <= 0) {
// 			// 半径0は点描画
// 			DrawPixel(_cx, _cy, _cc);
// 			return;
// 		}
// 		// 中点円アルゴリズムで輪郭点の左右最小/最大を収集し、スキャンライン塗りつぶし
// 		const int size = 2 * _r + 1;	// 直径分のオフセット数
// 		vector<int> left(size, INT_MAX);	// 各オフセットの左端X座標（中心基準）
// 		vector<int> right(size, INT_MIN);	// 各オフセットの右端X座標（中心基準）
//
// 		// 中点円アルゴリズム
// 		int x = _r;
// 		int y = 0;
// 		int p = 1 - _r;
//
// 		// オフセットに対応する左右端を更新するラムダ関数
// 		auto update = [&](int px, int py) {
// 			int off = (py - _cy) + _r; // 0..2r
// 			if (off < 0 || off >= size) return;
// 			int rx = px - _cx;
// 			if (rx < left[off]) left[off] = rx;
// 			if (rx > right[off]) right[off] = rx;
// 			};
//
// 		// 各八分円の点で左右端を更新
// 		while (x >= y) {
// 			update(_cx + x, _cy + y);	// 第1オクタント
// 			update(_cx - x, _cy + y);	// 第2オクタント
// 			update(_cx + x, _cy - y);	// 第8オクタント
// 			update(_cx - x, _cy - y);	// 第7オクタント
// 			update(_cx + y, _cy + x);	// 第4オクタント
// 			update(_cx - y, _cy + x);	// 第3オクタント
// 			update(_cx + y, _cy - x);	// 第5オクタント
// 			update(_cx - y, _cy - x);	// 第6オクタント
// 			// 次の点へ
// 			y++;
// 			if (p <= 0) {
// 				p = p + 2 * y + 1;
// 			}
// 			else {
// 				x--;
// 				p = p + 2 * y - 2 * x + 1;
// 			}
// 		}
// 		// スキャンライン塗りつぶし
// 		for (int off = 0; off < size; off++) {
// 			if (left[off] <= right[off]) {
// 				int py = _cy + (off - _r);
// 				for (int rx = left[off]; rx <= right[off]; rx++) {
// 					DrawPixel(_cx + rx, py, _cc);
// 				}
// 			}
// 		}
// 		return;
// 	}
// 	// 枠線（非塗りつぶし）
// 	int x = _r;
// 	int y = 0;
// 	int p = 1 - _r;
// 	// 中点円アルゴリズム
// 	while (x >= y) {
// 		// 各八分円の点を描画
// 		DrawPixel(_cx + x, _cy + y, _cc);	// 第1オクタント
// 		DrawPixel(_cx - x, _cy + y, _cc);	// 第2オクタント
// 		DrawPixel(_cx + x, _cy - y, _cc);	// 第8オクタント
// 		DrawPixel(_cx - x, _cy - y, _cc);	// 第7オクタント
// 		DrawPixel(_cx + y, _cy + x, _cc);	// 第4オクタント
// 		DrawPixel(_cx - y, _cy + x, _cc);	// 第3オクタント
// 		DrawPixel(_cx + y, _cy - x, _cc);	// 第5オクタント
// 		DrawPixel(_cx - y, _cy - x, _cc);	// 第6オクタント
// 		// 次の点へ
// 		y++;
// 		if (p <= 0) {
// 			p = p + 2 * y + 1;
// 		}
// 		else {
// 			x--;
// 			p = p + 2 * y - 2 * x + 1;
// 		}
// 	}
// }	// DrawCircle
//
// /**
// * 三角形を描く関数
// * @param _x1	頂点1 X座標
// * @param _y1	頂点1 Y座標
// * @param _x2	頂点2 X座標
// * @param _y2	頂点2 Y座標
// * @param _x3	頂点3 X座標
// * @param _y3	頂点3 Y座標
// * @param _cc	色コード
// * @param _fill	塗りつぶしフラグ（true: 塗りつぶし、false: 枠線のみ）
// */
// void DrawTriangle(int _x1, int _y1, int _x2, int _y2, int _x3, int _y3, int _cc, bool _fill)
// {
// 	// 塗りつぶし
// 	if (_fill) {
// 		// 頂点をY座標でソート（_y1 <= _y2 <= _y3）
// 		if (_y1 > _y2) { swap(_x1, _x2); swap(_y1, _y2); }
// 		if (_y1 > _y3) { swap(_x1, _x3); swap(_y1, _y3); }
// 		if (_y2 > _y3) { swap(_x2, _x3); swap(_y2, _y3); }
// 		// スキャンライン塗りつぶし
// 		// 各エッジのX座標をY座標に線形補間するラムダ関数
// 		auto EdgeInterpolate = [](int y, int y0, int x0, int y1, int x1) {
// 			if (y1 == y0) return x0;
// 			return x0 + (x1 - x0) * (y - y0) / (y1 - y0);
// 			};
// 		// スキャンラインごとにX座標の開始/終了を計算してピクセルを描画
// 		for (int y = _y1; y <= _y3; y++) {
// 			int x_start, x_end;
// 			if (y < _y2) {
// 				// 上半分
// 				x_start = EdgeInterpolate(y, _y1, _x1, _y3, _x3);
// 				x_end = EdgeInterpolate(y, _y1, _x1, _y2, _x2);
// 			}
// 			else {
// 				// 下半分
// 				x_start = EdgeInterpolate(y, _y1, _x1, _y3, _x3);
// 				x_end = EdgeInterpolate(y, _y2, _x2, _y3, _x3);
// 			}
// 			// X座標の大小を確認して描画
// 			if (x_start > x_end) swap(x_start, x_end);
// 			// ピクセル描画（両端含む）
// 			for (int x = x_start; x <= x_end; x++) {
// 				DrawPixel(x, y, _cc);
// 			}
// 		}
// 		return;
// 	}
// 	// 枠線（非塗りつぶし）
// 	DrawLine(_x1, _y1, _x2, _y2, _cc);	// 辺1
// 	DrawLine(_x2, _y2, _x3, _y3, _cc);	// 辺2
// 	DrawLine(_x3, _y3, _x1, _y1, _cc);	// 辺3
// }	// DrawTriangle
//
// /**
// * 楕円を描く関数（回転付き）
// * @param _cx	中心X座標
// * @param _cy	中心Y座標
// * @param _rx	半径X方向
// * @param _ry	半径Y方向
// * @param _angleDeg	回転角度（度）
// * @param _cc	色コード
// * @param _fill	塗りつぶしフラグ（true: 塗りつぶし、false: 枠線のみ）
// */
// void DrawEllipse(int _cx, int _cy, int _rx, int _ry, float _angleDeg, int _cc, bool _fill)
// {
// 	// ラジアン変換
// 	const double ang = _angleDeg * (M_PI / 180.0);
// 	const double cosA = cos(ang);
// 	const double sinA = sin(ang);
//
// 	// 塗りつぶし：逆写像方式（画面上の各ピクセルを逆回転して楕円内判定）
// 	if (_fill) {
// 		if (_rx <= 0 && _ry <= 0) {
// 			DrawPixel(_cx, _cy, _cc);
// 			return;
// 		}
// 		// 回転後の軸アラインド境界ボックス幅/高さ（切り上げ）
// 		double absCos = fabs(cosA), absSin = fabs(sinA);
// 		int halfW = (int)ceil(absCos * _rx + absSin * _ry);
// 		int halfH = (int)ceil(absSin * _rx + absCos * _ry);
// 		int x0 = _cx - halfW;
// 		int x1 = _cx + halfW;
// 		int y0 = _cy - halfH;
// 		int y1 = _cy + halfH;
//
// 		const double rx2 = (double)_rx * (double)_rx;
// 		const double ry2 = (double)_ry * (double)_ry;
//
// 		for (int sy = y0; sy <= y1; ++sy) {
// 			for (int sx = x0; sx <= x1; ++sx) {
// 				// スクリーン座標を中心基準に
// 				double dx = (double)sx - (double)_cx;
// 				double dy = (double)sy - (double)_cy;
// 				// 逆回転（-ang）して局所座標 (lx, ly) を得る
// 				double lx = dx * cosA + dy * sinA;
// 				double ly = -dx * sinA + dy * cosA;
// 				// 楕円内部判定（包括的に <= 1.0）
// 				double v = (lx * lx) / rx2 + (ly * ly) / ry2;
// 				if (v <= 1.0) {
// 					DrawPixel(sx, sy, _cc);
// 				}
// 			}
// 		}
// 		return;
// 	}
//
// 	// 枠線（非塗りつぶし）：パラメータ t をサンプリングして点列を生成、隙間ができないよう線で繋ぐ
// 	if (_rx <= 0 && _ry <= 0) {
// 		DrawPixel(_cx, _cy, _cc);
// 		return;
// 	}
// 	int steps = max(64, (int)((sqrt((double)(_rx * _rx + _ry * _ry)) * 4.0)));
// 	double twoPi = 2.0 * M_PI;
// 	int prevX = 0, prevY = 0;
// 	for (int i = 0; i <= steps; i++) {
// 		double t = twoPi * (double)i / (double)steps;
// 		double lx = (double)_rx * cos(t);
// 		double ly = (double)_ry * sin(t);
// 		// 回転してスクリーン座標へ
// 		int sx = (int)floor((double)_cx + (lx * cosA - ly * sinA) + 0.5);
// 		int sy = (int)floor((double)_cy + (lx * sinA + ly * cosA) + 0.5);
// 		if (i > 0) {
// 			DrawLine(prevX, prevY, sx, sy, _cc);
// 		}
// 		else {
// 			DrawPixel(sx, sy, _cc);
// 		}
// 		prevX = sx; prevY = sy;
// 	}
// }	// DrawEllipse
//
// /**
// * @brief 十字を描く関数
// * @param _cx	中心X座標
// * @param _cy	中心Y座標
// * @param _size	十字のサイズ
// * @param _cc	色コード
// */
// void DrawCross(int _cx, int _cy, int _size, int _cc)
// {
// 	// 十字を描く
// 	DrawLine(_cx - _size / 2, _cy, _cx + _size / 2, _cy, _cc);	// 横線
// 	DrawLine(_cx, _cy - _size / 2, _cx, _cy + _size / 2, _cc);	// 縦線
// }	// DrawCross
//
// /**
// * 2025 HAL Osaka. All rights reserved.
// */
