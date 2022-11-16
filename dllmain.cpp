// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "pch.h"

// SFAPI & GAMEAPI
#include "SAMPFUNCS_API.h"
#include "game_api.h"

SAMPFUNCS* SF = new SAMPFUNCS();

bool state = true;

/* 指令处理回调 */
void CALLBACK CommandHandler(std::string str)
{
    if (state)
    {
        state = false;
        SF->getSAMP()->getChat()->AddChatMessage(-1, "{A4EC2F}[Tr_自由城双开]: {2FEC96}关闭！");
    }
    else
    {
        state = true;
        SF->getSAMP()->getChat()->AddChatMessage(-1, "{A4EC2F}[Tr_自由城双开]: {2FEC96}开启！");
    }
}

/* 传出RPC包 */
bool CALLBACK IN_RPC(stRakNetHookParams* params)
{
    if (params->packetId == RPC_ScrShowTextDraw && state == true)
    {
        UINT16 TextDrawID, ModelID, Color1, Color2, TextLen;
        UINT8 Flags, Shadow, Outline, Style, Selectable;
        float LetterWidth, LetterHeight, LineWidth, LineHeight, X, Y, RotX, RotY, RotZ, Zoom;
        UINT32 LetterColor, BoxColor, BackgroundColor;
        char Text[4];

        params->bitStream->Read(TextDrawID);
        params->bitStream->Read(Flags);
        params->bitStream->Read(LetterWidth);
        params->bitStream->Read(LetterHeight);
        params->bitStream->Read(LetterColor);
        params->bitStream->Read(LineWidth);
        params->bitStream->Read(LineHeight);
        params->bitStream->Read(BoxColor);
        params->bitStream->Read(Shadow);
        params->bitStream->Read(Outline);
        params->bitStream->Read(BackgroundColor);
        params->bitStream->Read(Style);
        params->bitStream->Read(Selectable);
        params->bitStream->Read(X);
        params->bitStream->Read(Y);
        params->bitStream->Read(ModelID);
        params->bitStream->Read(RotX);
        params->bitStream->Read(RotY);
        params->bitStream->Read(RotZ);
        params->bitStream->Read(Zoom);
        params->bitStream->Read(Color1);
        params->bitStream->Read(Color2);
        params->bitStream->Read(TextLen);
        params->bitStream->Read(Text, TextLen);

        // 截取 @Text 前3位单元格 获取最后2位
        strncpy_s(Text, Text + 3, 2);
        if (SF->getSAMP()->getDialog()->DialogID == 1956)
        {
            SF->getSAMP()->sendDialogResponse(1956, 1, 0, Text);
            SF->getSAMP()->getChat()->AddChatMessage(-1, "{A4EC2F}[Tr_自由城双开]: {2FEC96}已为您自动填写激活码，可继续进行游戏！");
        }
    }
    return true;
}

/* 主循环 */
void CALLBACK ClassMain()
{
    static bool InitPlugin = false; // @bool 声明初始化变量

    if (!InitPlugin)
    {
        if (!GAME || GAME->GetSystemState() != eSystemState::GS_PLAYING_GAME) return;
        if (!SF->getSAMP()->IsInitialized()) return;

        SF->getSAMP()->registerChatCommand("tr.sk", CommandHandler);
        SF->getRakNet()->registerRakNetCallback(RAKHOOK_TYPE_INCOMING_RPC, IN_RPC);
        SF->getSAMP()->getChat()->AddChatMessage(-1, "{A4EC2F}[Tr_自由城双开]: {2FEC96}加载完成，作者: Weidu");
        InitPlugin = true;
    }
}

/* 核心入口事件 */
BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        SF->initPlugin(ClassMain, hModule); // 初始化插件
        break;
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

