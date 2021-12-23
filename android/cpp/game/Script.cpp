#include "Script.h"

int language;

Script script[LanguageMax] = {
		//Kor
		{
				{
						"Avoid Ball",
						"김민수의 포트폴리오입니다",
						"아무 키나 누르세요",

						"일시정지",

						"게임 끝",
						"기록 : %.2f",
						"최고 기록 : %.2f",
						"움직여주세요...",

						"인트로로 가시겠습니까?",
						"인트로로",
						"가시겠습니까?",

						"메뉴로 가시겠습니까?",
						"메뉴로",
						"가시겠습니까?",

						"게임을 다시 시작하시겠습니까?",
						"게임을 다시",
						"시작하시겠습니까?",

						"종료하시겠습니까?",

						"이전 플레이 기록이 없습니다 튜토리얼을 하시겠습니까?",
						"이전 플레이 기록이 없습니다",
						"튜토리얼을 하시겠습니까?",

						"튜토리얼을 모두 끝냈습니다 게임을 시작하시겠습니까?",
						"튜토리얼을 모두 끝냈습니다",
						"게임을 시작하시겠습니까?",

						"게임 준비중..."
				},
				{
						"기본 규칙",
						"목숨이 0이되면 게임이 끝남",
						"에너지를 모두 사용하면 빠르게 움직일 수 없음",
						"일정 시간이 지날때 마다 조금씩 어려워 짐",
						"날아오는 적으로 부터 오래 살아남으면 됨",
				},
				{
						"적의 이동",
						"직선 이동 : 직선으로 날아옴",
						"곡선 이동 : 곡선으로 날아옴",
						"적의 속도",
						"기본 : 일정한 속도로 날아옴",
						"변속 : 속도가 변하면서 날아옴",
				},
				{
						"적의 종류",
						": 맞으면 체력 깎임",
						": 맞으면 느려짐",
						": 맞으면 시야 제한",
						": 맞으면 3초마다 체력 깎임",
						": 3대 맞으면 즉사",
						": 맞으면 최대 기력 줄어듬",
				},
				{
						"아이템",
						"초록색 공에 맞으면 생성(최대 1개)",
						"먹으면 3초마다 체력이 줄어드는 것이 멈춤",
						"노란색 공에 맞으면 생성(최대 4개)",
						"줄어든 최대 에너지가 다시 늘어남",
				},
				{
						"키 조작",
						"왼쪽 패드 : 상, 하, 좌, 우 이동",
						"오른쪽 버튼 : 빠르게 움직임",
						"(움직이는 중에만 빠르게 움직일 수 있음)",
						"esc : 일시 정지",
						"F1 : 전체화면",
				},
				{
						"언어 선택",
						"한국어",
						"배경음",
						"효과음",
						"켜짐",
						"꺼짐",
						"전체 화면",
						"화면 필터"
				},
				{
						"게임시작",
						"튜토리얼",
						"정보",
						"설정",
						"인트로",
						"게임 종료",
						"필터변경",

						"계속하기",
						"다시하기",
						"메뉴",

						"기록 초기화",
						"Skip",
				},
				{
						"왼쪽 아래의 패드를 움직여 보세요",
						"오른쪽 아래의 버튼 눌러 빠르게 움직여 보세요",
						"맞지말고 피해보세요",

						"Clear"
				},
		},
		//En
		{
				{
						"Avoid Ball",
						"This is MinSu's portfolio",
						"Press Any Key",

						"Pause",

						"Game Over",
						"Record : %.2f",
						"Best Record : %.2f",
						"Please move...",

						"Do you want to go to the intro?",
						"Do you want to go",
						"with the intro?",

						"Do you want to go to the menu?",
						"Do you want to go",
						"to the menu?",

						"Do you want to start the game again?",
						"Do you want to start",
						"the game again?",

						"Do you want to end it?",

						"No previous record of playing record. Play tutorial?",
						"No previous record of playing record",
						"Play tutorial?",

						"Finished the tutorial. Start the game?",
						"Finished the tutorial",
						"Start the game?",

						"Getting ready..."
				},
				{
						"Rules",
						"Hp zero, the game is over",
						"Use all energy, can't move fast",
						"As time goes by, it's getting harder",
						"Survive for a long time",
				},
				{
						"Enemy's movement",
						"Straight line movement",
						"Curve movement",

						"Speed of the enemy",
						"Basics : It flew at a constant speed",
						"Shift : As the speed changes, it flies",
				},
				{
						"Types of enemies",
						": Hp down",
						": Slow down",
						": Limitation of vision",
						": Less Hp after 3 seconds",
						": Hit three times, you die",
						": Lose maximum energy",
				},
				{
						"Item",
						"Hit by a green ball create one(Max One)",
						"Stops decreasing HP every 3 seconds",
						"Hit by a yellow ball create one(Max Four)",
						"Reduced maximum energy is increased again",
				},
				{
						"Key",
						"Left pad : Up, Down, Left, Right",
						"Right button : Move fast",
						"(Only move fast while moving)",
						"esc : pause",
						"F1 : Full screen",
				},
				{
						"Language",
						"English",
						"BGM",
						"SFX",
						"On",
						"Off",
						"Fill Screen",
						"Screen Filter"
				},
				{
						"Start",
						"tutorial",
						"Info",
						"Setting",
						"Intro",
						"Quit",
						"Chage Filter",

						"Continue",
						"Restart",
						"Menu",

						"Record Reset",
						"Skip",
				},
				{
						"Press bottom left pad to move",
						"Press bottom right button to quickly move",
						"Don't get hit and avoid it",

						"Clear"
				},
		},
		//Jp
		{
				{
						"Avoid Ball",
						"ミンスのポートフォリオです",
						"どのキーでもおすください",

						"一時停止",

						"ゲーム終了",
						"記録 : %.2f",
						"最高 記録 : %.2f",
						"動いてください...",

						"イントロで行きますか?",
						"イントロで",
						"行きますか?",

						"メニューになさいますか?",
						"メニューに",
						"なさいますか?",

						"ゲームをまた始めますか?",
						"ゲームをまた",
						"始めますか?",

						"終了しますか?",

						"以前のプレイ記録がありません チュートリアルをしますか?",
						"以前のプレイ記録がありません",
						"チュートリアルをしますか?",

						"チュートリアルを全部終わらせました. ゲームを開始しますか?",
						"チュートリアルを全部終わらせました",
						"ゲームを開始しますか?",

						"ゲーム準備中..."
				},
				{
						"基本ルール",
						"命が0になったらゲーム終了",
						"気力を使い果たすと素早く動けない",
						"日程時間が過ぎるたびに少しずつ難しくなる",
						"飛んでくる敵から長く生き残ればいい",
				},
				{
						"敵の移動",
						"直線移動 : 直線に飛んでくる",
						"曲線移動 : 曲線に飛来",

						"敵のスピード",
						"基本 : 一定速度で飛んでくる",
						"変速 : 速度が変わりながら飛来",
				},
				{
						"敵の種類",
						": 当たれば体力を落とす",
						": 当たれば遅くなる",
						": 当たれば視野制限",
						": 当たれば3秒ごとに体力を落とす",
						": 3発殴られたら即死",
						": 当たると最大気力が減る",
				},
				{
						"アイテム",
						"グリーンボールに当たれば作成(最大1個)",
						"食べると三秒毎に体力の縮みが止まる",
						"黄色ボールに当たると生成（最大4個）",
						"減少した最大エネルギーが再び増加",
				},
				{
						"キー操作",
						"左パッド : 上·下·左·右 移動",
						"右ボタン : すばやい動き",
						"(動いている最中のみ素早く動ける)",
						"esc : 一時停止",
						"F1 : 全画面",
				},
				{
						"言語",
						"日本語",
						"背景音",
						"効果音",
						"オン",
						"オフ",
						"全画面",
						"画面 フィルタ"
				},
				{
						"スタート",
						"チュートリアル",
						"情報",
						"設定",
						"イントロ",
						"終了",
						"フィルタの変更",

						"継続",
						"やり直す",
						"メニュー",

						"記録初期化",
						"Skip",
				},
				{
						"左下のパッドを押して動かしてみてください",
						"右下のボタンを押して速く動いてみてください",
						"当たらずに避けてみてください",

						"Clear"
				},
		},
};

Btn btn = {
		"O", "X", "<", ">"
};
