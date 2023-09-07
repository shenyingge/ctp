#pragma once

#include "my_data_type.h"
#include "../api/ThostFtdcTraderApi.h"
#include "../api/ThostFtdcMdApi.h"

#include<string>
#include<map>
#include<vector>
#include<mutex>
#include<condition_variable>
#include<thread>

// #include<mysql.h>


using namespace std;


//保存期货合约及相关信息的结构体，由tradeApi读取后填入
struct MyInstrumentInfoDataStruct {
    string instrumentId;
    string exchangeId;
    double volumeMultiplier;
    double longMarginRatio;
    double shortMarginRatio;
    double priceTick;
};


//账号登录信息结构体
struct MyAccountDataStruct {

    // 接口使用这需要填入的数据
    TThostFtdcInvestorIDType investorID;
    TThostFtdcPasswordType password;
    TThostFtdcBrokerIDType brokerID;
    TThostFtdcAppIDType appID;
    TThostFtdcAuthCodeType authCode;

    char marketDataFrontAddr[30];
    char tradeFrontAddr[30];

    bool bIsTradeApiConnected;
    map<string, MyInstrumentInfoDataStruct*> map_allAvailableInstrumentInfo;
    CThostFtdcTraderApi *pTradeApi;

};


// //保存期货合约及相关信息的结构体，由tradeApi读取后填入
// struct MyInstrumentInfoDataStruct {
//     string instrumentID;
//     string exchangeID;
//     double volumeMultiplier;
//     double longMarginRatio;
//     double shortMarginRatio;
//     double priceTick;
// };

