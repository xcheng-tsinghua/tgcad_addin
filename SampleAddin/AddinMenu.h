#pragma once

#include <vector>
#include <string>
#include "SampleAddin.h"

#include "SEAddin.h"
#include "SDK/include/secatids.h"
#include "Resource.h"


// Define the command IDs
enum MenuFunc
{
	idSave1,
	idSave2,
	idSave3,
	idFolder,
	idMonitor,
	idBox,
	idCamera,
	idPhotograph,
	idFavorites,
	idPrinter,
	idTools,
	idCommandPrompt,
	idNotepad,
	idHelp,
	idSearch,
	idQuestion,
	idCheckbox1,
	idCheckbox2,
	idCheckbox3,
	idRadiobutton1,
	idRadiobutton2,
	idRadiobutton3,
	idBoundingBox,
	idBoxes,
	idGdiPlus,

	//NDS CAD ADD
	idLogout,
	idExport,
	idDataMgr,
	idCustomPage,
	idCustomCommand,
	idCustomDrawBox,
	idDraw,

	// user
	idTest1,
	idTest2,
	idTest3,
	idTest4,
	idTest5,
	idTest6,
	idTest7,

};

typedef struct MY_COMMAND_INFO_
{
	UINT			iCategory;
	UINT			iGroup;
	UINT			iCommand;
	UINT			iString;
	UINT			iImage;
	SeButtonStyle	buttonStyle;
} MY_COMMAND_INFO, * PMY_COMMAND_INFO;

typedef struct MY_ENVIRONMENT_INFO_
{
	GUID				environmentGuid;
	MY_COMMAND_INFO* pCommands;
	UINT				nCommands;
} MY_ENVIRONMENT_INFO, * PMY_ENVIRONMENT_INFO;

static const MY_COMMAND_INFO MyCommands3D[] =
{
	// ◊‹≤Âº˛√˚-----------≤Âº˛◊È√˚----------------------π¶ƒ‹id-------------π¶ƒ‹√˚----------------Õº±Í---------------------—˘ Ω
	//{ IDS_CMD_CATEGORY, IDS_CMD_GROUP_MODELTESTTOOLS , idTest1, IDS_CMD_SKETCHING , IDC_SKETCHING, seButtonIconAndCaptionBelow },// ≥ı ºªØ
	//{ IDS_CMD_CATEGORY, IDS_CMD_GROUP_MODELTESTTOOLS , idTest2, IDS_CMD_SKETCHING , IDC_SKETCHING, seButtonIconAndCaptionBelow },// ≥ı ºªØ
	//{ IDS_CMD_CATEGORY, IDS_CMD_GROUP_MODELTESTTOOLS , idCustomPage, IDS_CMD_DATABASECHECK , IDB_DATABASECHECK, seButtonIconAndCaptionBelow },// ≥ı ºªØ
	//{ IDS_CMD_CATEGORY, IDS_CMD_GROUP_MODELTESTTOOLS , idCustomCommand, IDS_CMD_MODELINGCHECK , IDB_MODELINGCHECK, seButtonIconAndCaptionBelow },// ª≠≤›Õº
	//{ IDS_CMD_CATEGORY, IDS_CMD_GROUP_MODELTESTTOOLS , idCustomDrawBox, IDS_CMD_DRAW_BOX , IDB_MODELINGCHECK, seButtonIconAndCaptionBelow },// «Âø’
	//{ IDS_CMD_CATEGORY, IDS_CMD_GROUP_MODELTESTTOOLS , idDraw, IDS_CMD_SKETCH_DEMO , IDB_MODELINGCHECK, seButtonIconAndCaptionBelow },// ≤‚ ‘1
	//{ IDS_CMD_CATEGORY, IDS_CMD_GROUP_MODELTESTTOOLS , idSolid, IDS_CMD_SOLID_DEMO , IDB_MODELINGCHECK, seButtonIconAndCaptionBelow },// ≤‚ ‘2

	//{ IDS_CMD_CATEGORY, IDS_CMD_GROUP_MODELTESTTOOLS , idTest3, IDS_CMD_TEST3 , IDB_MODELINGCHECK, seButtonIconAndCaptionBelow },// ≤‚ ‘3
	//{ IDS_CMD_CATEGORY, IDS_CMD_GROUP_MODELTESTTOOLS , idTest4, IDS_CMD_TEST4 , IDB_MODELINGCHECK, seButtonIconAndCaptionBelow },// ≤‚ ‘3
	//{ IDS_CMD_CATEGORY, IDS_CMD_GROUP_MODELTESTTOOLS , idTest5, IDS_CMD_TEST5 , IDB_MODELINGCHECK, seButtonIconAndCaptionBelow },// ≤‚ ‘4
	//{ IDS_CMD_CATEGORY, IDS_CMD_GROUP_MODELTESTTOOLS , idTest6, IDS_CMD_TEST6 , IDC_SKETCHING, seButtonIconAndCaptionBelow },// ≤‚ ‘5
	//{ IDS_CMD_CATEGORY, IDS_CMD_GROUP_MODELTESTTOOLS , idTest7, IDS_CMD_TEST7 , IDB_MODELINGCHECK, seButtonIconAndCaptionBelow },// ≤‚ ‘7

	// 3D≤›Õº
	//{ IDS_CMD_CATEGORY, IDS_CMD_GROUP_MODELTESTTOOLS , id3DSketch, IDS_CMD_3DSKETCH , IDB_MODELINGCHECK, seButtonIconAndCaptionBelow },// ≤‚ ‘7

		// backbone
	{ IDS_CMD_CATEGORY, IDS_CMD_GROUP_MODELTESTTOOLS , idTest1, IDS_CMD_BN1 , IDB_MODELINGCHECK, seButtonIconAndCaptionBelow },// ≤‚ ‘3
	{ IDS_CMD_CATEGORY, IDS_CMD_GROUP_MODELTESTTOOLS , idTest2, IDS_CMD_BN2 , IDB_MODELINGCHECK, seButtonIconAndCaptionBelow },// ≤‚ ‘3
	{ IDS_CMD_CATEGORY, IDS_CMD_GROUP_MODELTESTTOOLS , idTest3, IDS_CMD_BN3 , IDB_MODELINGCHECK, seButtonIconAndCaptionBelow },// ≤‚ ‘3
	{ IDS_CMD_CATEGORY, IDS_CMD_GROUP_MODELTESTTOOLS , idTest4, IDS_CMD_BN4 , IDB_MODELINGCHECK, seButtonIconAndCaptionBelow },// ≤‚ ‘3
	{ IDS_CMD_CATEGORY, IDS_CMD_GROUP_MODELTESTTOOLS , idTest5, IDS_CMD_BN5 , IDB_MODELINGCHECK, seButtonIconAndCaptionBelow },// ≤‚ ‘4

};

static const MY_COMMAND_INFO MyCommands2D[] =
{
	{ 0, 0, idSave1, 0, 0, seButtonAutomatic },

	//{ IDS_CMD_CATEGORY1, IDS_CMD_GROUP1, idSave1, IDS_CMD_SAVE1, IDB_SAVE, seButtonAutomatic },
	//{ IDS_CMD_CATEGORY1, IDS_CMD_GROUP1, idSave2, IDS_CMD_SAVE2, IDB_SAVE, seButtonAutomatic },
	//{ IDS_CMD_CATEGORY1, IDS_CMD_GROUP1, idSave3, IDS_CMD_SAVE3, IDB_SAVE, seButtonAutomatic },
	//{ IDS_CMD_CATEGORY1, IDS_CMD_GROUP1, idFolder, IDS_CMD_FOLDER, IDB_FOLDER, seButtonAutomatic },
	//{ IDS_CMD_CATEGORY1, IDS_CMD_GROUP1, idMonitor, IDS_CMD_MONITOR, IDB_MONITOR, seButtonAutomatic },
	//{ IDS_CMD_CATEGORY1, IDS_CMD_GROUP1, idBox, IDS_CMD_BOX, IDB_BOX, seButtonIconAndCaptionBelow },
	//{ IDS_CMD_CATEGORY1, IDS_CMD_GROUP2, idCamera, IDS_CMD_CAMERA, IDB_CAMERA, seButtonIconAndCaptionBelow },
	//{ IDS_CMD_CATEGORY1, IDS_CMD_GROUP2, idPhotograph, IDS_CMD_PHOTOGRAPH, IDB_PHOTOGRAPH, seButtonIconAndCaptionBelow },
	//{ IDS_CMD_CATEGORY1, IDS_CMD_GROUP2, idFavorites, IDS_CMD_FAVORITES, IDB_FAVORITES, seButtonIconAndCaptionBelow },
	//{ IDS_CMD_CATEGORY1, IDS_CMD_GROUP2, idPrinter, IDS_CMD_PRINTER, IDB_PRINTER, seButtonIconAndCaptionBelow },
	//{ IDS_CMD_CATEGORY1, IDS_CMD_GROUP3, idTools, IDS_CMD_TOOLS, IDB_TOOLS, seButtonIconAndCaptionBelow },
	//{ IDS_CMD_CATEGORY1, IDS_CMD_GROUP3, idCommandPrompt, IDS_CMD_COMMAND_PROMPT, IDB_COMMAND_PROMPT, seButtonIconAndCaptionBelow },
	//{ IDS_CMD_CATEGORY1, IDS_CMD_GROUP3, idNotepad, IDS_CMD_NOTEPAD, IDB_NOTEPAD, seButtonIconAndCaptionBelow },
	//{ IDS_CMD_CATEGORY1, IDS_CMD_GROUP4, idHelp, IDS_CMD_HELP, IDB_HELP, seButtonIconAndCaptionBelow },
	//{ IDS_CMD_CATEGORY1, IDS_CMD_GROUP4, idSearch, IDS_CMD_SEARCH, IDB_SEARCH, seButtonIconAndCaptionBelow },
	//{ IDS_CMD_CATEGORY1, IDS_CMD_GROUP5, idQuestion, IDS_CMD_QUESTION, IDB_QUESTION, seButtonIconAndCaptionBelow },
	//{ IDS_CMD_CATEGORY1, IDS_CMD_GROUP6, idCheckbox1, IDS_CMD_CHECKBOX1, 0, seCheckButton },
	//{ IDS_CMD_CATEGORY1, IDS_CMD_GROUP6, idCheckbox2, IDS_CMD_CHECKBOX2, 0, seCheckButton },
	//{ IDS_CMD_CATEGORY1, IDS_CMD_GROUP6, idCheckbox3, IDS_CMD_CHECKBOX3, 0, seCheckButton },
	//{ IDS_CMD_CATEGORY1, IDS_CMD_GROUP6, idRadiobutton1, IDS_CMD_RADIOBUTTON_1, 0, seRadioButton },
	//{ IDS_CMD_CATEGORY1, IDS_CMD_GROUP6, idRadiobutton2, IDS_CMD_RADIOBUTTON_2, 0, seRadioButton },
	//{ IDS_CMD_CATEGORY1, IDS_CMD_GROUP6, idRadiobutton3, IDS_CMD_RADIOBUTTON_3, 0, seRadioButton }
};


static const MY_ENVIRONMENT_INFO MyEnvironments[] =
{
	{ CATID_SEAssembly, (MY_COMMAND_INFO*)&MyCommands3D, _countof(MyCommands3D) },
	{ CATID_SEDraft, (MY_COMMAND_INFO*)&MyCommands3D, _countof(MyCommands3D) },
	{ CATID_SEPart, (MY_COMMAND_INFO*)&MyCommands3D, _countof(MyCommands3D) },
	{ CATID_SEDMPart, (MY_COMMAND_INFO*)&MyCommands3D, _countof(MyCommands3D) },
	{ CATID_SESheetMetal, (MY_COMMAND_INFO*)&MyCommands3D, _countof(MyCommands3D) },
	{ CATID_SEDMSheetMetal, (MY_COMMAND_INFO*)&MyCommands3D, _countof(MyCommands3D) }
};



class AddinMenu
{
public:
	AddinMenu();
	virtual ~AddinMenu();

public:

	static bool CallCommand(MenuFunc id);

	static bool Uninit();
	static bool GetSolidEdgeType(IDispatchPtr disp, SolidEdgeConstants::ObjectType& getType);

	static int SetPartColor(SolidEdgePart::PartDocumentPtr pPartDoc, const std::string& strStyle);

	static int SetBodyColor(SolidEdgeGeometry::BodyPtr pBody, FaceStylePtr pStyle);

private:
	static bool CallCustomPage();
	static bool CallCustomCommand();
	static bool CallCreatBoxCommand();

	//≤›Õº
	static bool CallCreateLine();

	static bool CallCreateCircle();

	static bool CallCreateRectangle();

	static bool CallCreateEllipse();

	static void CreateHoles(SolidEdgePart::PartDocumentPtr partDocument);

	static void CreateFiniteExtrudedProtrusion(SolidEdgePart::PartDocumentPtr partDocument);

	static void CreateBox(SolidEdgePart::PartDocumentPtr partDocument);

	//¿≠…Ï
	static bool  CreateExtrudedProtrusion();
	//‘≤Ω«
	static bool CreateRound();
	//µπΩ«
	static bool CreateChamfer();
	//–˝◊™¿≠…Ï
	static bool CreateRevolvedProtrusions();
	//–˝◊™«–∏Ó
	static bool CreateRevolvedCutouts();
	//∫œ≤¢
	static bool CreateUnions();
	//œ‡Ωª
	static bool CreateIntersects();
	//ºı»•
	static bool CreateSubtracts();

	static bool Test1();
	static bool Test2();
	static bool Test3();
	static bool Test4();
	static bool Test5();
	static bool Test6();
	static bool Test7();
	static bool HandSketch3D();

	//[zheng.wentong][2024.7.16]∆Ù∂Ø÷«ƒ‹≤›ªÊ
	static bool StartSketching();

	static bool ActiveMouse();

};





