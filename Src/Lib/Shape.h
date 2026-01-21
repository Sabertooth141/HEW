#pragma once
/**
* @file Shape.cpp/h
* @brief 2D図形描画関数群（実装）
* @revision	1
*
* 2025 HAL Osaka. All rights reserved.
*/
#define _USE_MATH_DEFINES	//M_PI定義のため
#include "conioex.h"
#include <iostream>
#include <cmath>
#include <vector>
#include <climits>

/**
* 線を描く関数
* @param _x1	始点X座標
* @param _y1	始点Y座標
* @param _x2	終点X座標
* @param _y2	終点Y座標
* @param _cc	色コード
*/
void DrawLine(int _x1, int _y1, int _x2, int _y2, int _cc);

/**
* 四角形を描く関数
* @param _x1	左上X座標
* @param _y1	左上Y座標
* @param _x2	右下X座標
* @param _y2	右下Y座標
* @param _cc	色コード
* @param _fill	塗りつぶしフラグ（true: 塗りつぶし、false: 枠線のみ）
*/
void DrawRect(int _x1, int _y1, int _x2, int _y2, int _cc, bool _fill = false);

/**
* 円を描く関数
* @param _cx	中心X座標
* @param _cy	中心Y座標
* @param _r	半径
* @param _cc	色コード
* @param _fill	塗りつぶしフラグ（true: 塗りつぶし、false: 枠線のみ）
*/
void DrawCircle(int _cx, int _cy, int _r, int _cc, bool _fill = false);

/**
* 三角形を描く関数
* @param _x1	頂点1 X座標
* @param _y1	頂点1 Y座標
* @param _x2	頂点2 X座標
* @param _y2	頂点2 Y座標
* @param _x3	頂点3 X座標
* @param _y3	頂点3 Y座標
* @param _cc	色コード
* @param _fill	塗りつぶしフラグ（true: 塗りつぶし、false: 枠線のみ）
*/
void DrawTriangle(int _x1, int _y1, int _x2, int _y2, int _x3, int _y3, int _cc, bool _fill = false);

/**
* 楕円を描く関数（回転付き）
* @param _cx	中心X座標
* @param _cy	中心Y座標
* @param _rx	半径X方向
* @param _ry	半径Y方向
* @param _angleDeg	回転角度（度）
* @param _cc	色コード
* @param _fill	塗りつぶしフラグ（true: 塗りつぶし、false: 枠線のみ）
*/
void DrawEllipse(int _cx, int _cy, int _rx, int _ry, float _angleDeg, int _cc, bool _fill = false);

/**
* @brief 十字を描く関数
* @param _cx	中心X座標
* @param _cy	中心Y座標
* @param _size	十字のサイズ
* @param _cc	色コード
*/
void DrawCross(int _cx, int _cy, int _size, int _cc);

/**
* 2025 HAL Osaka. All rights reserved.
*/
