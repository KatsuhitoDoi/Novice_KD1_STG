#include <Novice.h>

const char kWindowTitle[] = "Katsuhito_D_タイトル ";

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, 1280, 720);

	// キー入力結果を受け取る箱
	char keys[256] = { 0 };
	char preKeys[256] = { 0 };

	//変数宣言・初期化

	//プレイヤー
	int playerPosX = 640;
	int playerPosY = 560;
	int playerRadius = 16;
	int playerSpeed = 5;

	//弾
	int bulletPosX[10] = { 0 };
	int bulletPosY[10] = { 0 };
	int bulletRadius[10] = {8,8,8,8,8,8,8,8,8,8};
	int bulletSpeed[10] = { 10,10,10,10,10,10,10,10,10,10 };
	bool isBulletShot[10] = { true };

	int coolTime = 0;
	
	//敵
	int enemyPosX[10] = { 140,240,340,440,540,640,740,840,940,1040 };
	int enemyPosY[10] = { 100,150,200,250,300,300,250,200,150,100 };
	int enemyRadius[10] = { 20,20,20,20,20,20,20,20,20,20 };
	int enemySpeed[10] = { 2,2,2,2,2,2,2,2,2,2 };
	int enemyRespawnTimer = 100;
	bool isEnemyAlive[10] = { true };

	//敵に向かう弾
	int bullet2enemyX[10] = { 0 };
	int bullet2enemyY[10] = { 0 };
	//float bullet2enemy = 0.0f;

	//画像の読み込み

	//爆発
	int explosionGH =
		Novice::LoadTexture("./NoviceResources/explode2.png");

	//プレイヤー
	int playerGH =
		Novice::LoadTexture("./NoviceResources/fighter.png");

	//エネミー
	int enemyGH =
		Novice::LoadTexture("./NoviceResources/monster.png");


	// ウィンドウの×ボタンが押されるまでループ
	while (Novice::ProcessMessage() == 0) {
		// フレームの開始
		Novice::BeginFrame();

		// キー入力を受け取る
		memcpy(preKeys, keys, 256);
		Novice::GetHitKeyStateAll(keys);

		///
		/// ↓更新処理ここから
		///

		//プレイヤーの操作

		//Aキーまたは←キーを押したら、左に移動
		if (Novice::CheckHitKey(DIK_A) || Novice::CheckHitKey(DIK_LEFT)) {
			playerPosX -= playerSpeed;
		}

		//Dキーまたは→キーを押したら、右に移動
		if (Novice::CheckHitKey(DIK_D) || Novice::CheckHitKey(DIK_RIGHT)) {
			playerPosX += playerSpeed;
		}

		//Wキーまたは↑キーを押したら、上に移動
		if (Novice::CheckHitKey(DIK_W) || Novice::CheckHitKey(DIK_UP)) {
			playerPosY -= playerSpeed;
		}

		//Sキーまたは↓キーを押したら、下に移動
		if (Novice::CheckHitKey(DIK_S) || Novice::CheckHitKey(DIK_DOWN)) {
			playerPosY += playerSpeed;
		}

		//自キャラが画面外に出ないようにする処理

		//X軸
		if (playerPosX >= 1230) {
			playerPosX = 1230;
		}

		if (playerPosX <= 15) {
			playerPosX = 15;
		}

		//Y軸
		if (playerPosY >= 670) {
			playerPosY = 670;
		}

		if (playerPosY <= 15) {
			playerPosY = 15;
		}
		
		//弾の発射処理
		for (int i = 0; i <= 9; i++) {
			if (isBulletShot[i] == false) {
				if (keys[DIK_SPACE] && preKeys[DIK_SPACE] == 0 && coolTime <= 0) {
  					coolTime = 5;
					isBulletShot[i] = true;
					bulletPosX[i] = playerPosX + 15;//弾が中心から発射されるよう修正
					bulletPosY[i] = playerPosY;
				}
				

			}

		}
		
		//弾のクールタイムの計算
		if (coolTime >= 0) {
			coolTime--;
		}


		//弾道計算
		for (int i = 0; i <= 9; i++) {
			if (isBulletShot[i] == true) {
				bulletPosY[i] -= bulletSpeed[i];
				if (bulletPosY[i] <= 0) {
					bulletPosY[i] = -200;
					isBulletShot[i] = false;
				}
			}
		
		}




		

		//敵の移動
		for (int i = 0; i <= 9; i++) {
			if (isEnemyAlive) {
				//敵を動かす（横移動）
				enemyPosX[i] += enemySpeed[i];

				//右で反射
				if (enemyPosX[i] + enemyRadius[i] >= 1260) {
					enemySpeed[i] *= -1;
				}

				//左で反射
				if (enemyPosX[i] - enemyRadius[i] <= 20) {
					enemySpeed[i] *= -1;
				}
			}

		}
		

		//敵と弾の衝突判定
		for (int i = 0; i <= 9; i++) {
			if (isEnemyAlive[i] == true && isBulletShot[i] == true) {
				bullet2enemyX[i] = enemyPosX[i] - bulletPosX[i];
				bullet2enemyY[i] = enemyPosY[i] - bulletPosY[i];
				if ((enemyRadius[i] + bulletRadius[i]) * (enemyRadius[i] + bulletRadius[i])
					>= (bullet2enemyX[i] * bullet2enemyX[i]) + (bullet2enemyY[i] * bullet2enemyY[i])) {
					isEnemyAlive[i] = false;
					isBulletShot[i] = false;
				}
			}
		}
		
		
		

		//復活用タイマー（敵が死んでいたらタイマースタート）
		for (int i = 0; i <= 9; i++) {
			if (!isEnemyAlive[i]) {
				enemyRespawnTimer--;
				if (enemyRespawnTimer <= 0) {
					isEnemyAlive[i] = true;
					enemyRespawnTimer = 100;
				}
			}
		}


		


		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///

		//プレイヤーの描画（丸）
		/*Novice::DrawEllipse(
			playerPosX, playerPosY,
			playerRadius, playerRadius,
			0.0f,
			WHITE,
			kFillModeSolid);*/

			//プレイヤーの描画（画像）
		Novice::DrawSprite(
			static_cast<int>(playerPosX - playerRadius),
			static_cast<int>(playerPosY - playerRadius),
			playerGH,
			2.0f, 2.0f,
			0.0f,
			0xFFFFFFFF);


		//弾の描画（三角）
		for (int i = 0; i <= 9; i++) {
			if (isBulletShot[i] == true) {
				Novice::DrawTriangle(
					(int)bulletPosX[i],
					bulletPosY[i] - (int)bulletRadius[i],

					(int)bulletPosX[i] - (int)bulletRadius[i],
					(int)bulletPosY[i] + (int)bulletRadius[i],

					(int)bulletPosX[i] + (int)bulletRadius[i],
					(int)bulletPosY[i] + (int)bulletRadius[i],
					0xFFFF00FF, kFillModeSolid);
			}
		}


		

		//敵の描画（四角形）
		/*if (isEnemyAlive) {
			Novice::DrawBox(
				static_cast<int>(enemyPosX - enemyRadius),
				static_cast<int>(enemyPosY - enemyRadius),
				static_cast<int>(enemyRadius * 2),
				static_cast<int>(enemyRadius * 2),
				0.0f,
				0xFF8888FF,
				kFillModeSolid);
		}*/

		//敵の描画（画像）
		for (int i = 0; i <= 9; i++) {
			if (isEnemyAlive) {
				Novice::DrawSprite(
					static_cast<int>(enemyPosX[i] - enemyRadius[i]),
					static_cast<int>(enemyPosY[i] - enemyRadius[i]),
					enemyGH,
					2.0f, 2.0f,
					0.0f,
					0xFFFFFFFF);
			}

		}

		



		//爆発の描画
		for (int i = 0; i <= 9; i++) {
			if (!isEnemyAlive) {
				enemyRespawnTimer--;
				if (enemyRespawnTimer <= 100 || enemyRespawnTimer >= 75) {
					Novice::DrawSprite(
						static_cast<int>(enemyPosX[i] - enemyRadius[i]),
						static_cast<int>(enemyPosY[i] - enemyRadius[i]),
						explosionGH,
						2.0f, 2.0f,
						0.0f,
						0xFFFFFFFF
					);

				}
			}

		}


		

		///
		/// ↑描画処理ここまで
		///

		// フレームの終了
		Novice::EndFrame();

		// ESCキーが押されたらループを抜ける
		if (preKeys[DIK_ESCAPE] == 0 && keys[DIK_ESCAPE] != 0) {
			break;
		}
	}

	// ライブラリの終了
	Novice::Finalize();
	return 0;
}