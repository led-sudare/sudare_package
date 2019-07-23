#ifndef SUDARE_H__
#define SUDARE_H__

/** 直交座標定義 */
#define RECTANGULAR_WIDTH 30
#define RECTANGULAR_HEIGHT 100
#define RECTANGULAR_DEPTH 30

/** 極座標定義 */
#define POLAR_ANGLE_RESOLUTION 6
#define POLAR_ANGLES 360 / POLAR_ANGLE_RESOLUTION
#define POLAR_RADIUS 15
#define POLAR_HEIGHT 100

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/**
 SDK初期化
 @param[in] dst すだれデータの送信先 ex. 172.27.1.2:5510 or *:5511
 @return 成功したら0、失敗したら0以外を返す
 */
int sudare_init_sdk(const char* dst);

/**
 直行座標系でLEDを指定し色を設定する
 @param[in] x 0 <= x < RECTANGULAR_WIDTH
 @param[in] y 0 <= y < RECTANGULAR_HEIGHT
 @param[in] z 0 <= z < RECTANGULAR_DEPTH
 @param[in] rgb 0x000000 <= rgb <= 0xFFFFFF
 @note sudare_sendを呼ぶまで実機の点灯は切り替わらない
 @return 成功したら0、失敗したら0以外を返す
*/
int sudare_set_led_rect(int x, int y, int z, int rgb);

/**
 極座標系でLEDを指定し色を設定する
 @param[in] a 0 <= a < POLAR_ANGLES
 @param[in] r 0 <= r < POLAR_RADIUS
 @param[in] h 0 <= h < POLAR_HEIGHT
 @param[in] rgb 0x000000 <= rgb <= 0xFFFFFF
 @note sudare_sendを呼ぶまで実機の点灯は切り替わらない
 @return 成功したら0、失敗したら0以外を返す
*/
int sudare_set_led_polar(int a, int r, int h, int rgb);

/**
 全てのLEDを消す
 @note sudare_sendを呼ぶまで実機の点灯は切り替わらない
 @return 成功したら0、失敗したら0以外を返す
 */
int sudare_clear(void);

/**
 LEDすだれにデータ送信する<br>
 直前にSetLedRectを呼んだ場合は極座標系に変換してから送信する。<br>
 SetLedPolarを呼んだ場合はそのまま送信する。
 @return 成功したら0、失敗したら0以外を返す
*/
int sudare_send(void);

/**
 スリープする
 @param[in] ms ミリ秒
 @return 成功したら0、失敗したら0以外を返す
*/
int sudare_sleep(int ms);

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* SUDARE_H__ */
