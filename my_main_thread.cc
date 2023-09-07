#include "my/my_main_thread.h"
#include "my/my_ctp_api.h"
#include<fstream>
#include<iostream>

void MyMainThread::StartMainThread(){
    PrintNewMessage("ctp tick data store start.");

    // init pointers
    InitDataPointers();

    // load config
    ReadAcocuntSettingFile();
    ReadMysqlSettingFile();

    // add mysql table
    CreatMysqlTable();

    PrintNewMessage("start connect ctp.");

    ConnectToTrade_api();

    // wait for trade api connect
    while (true){
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        if (my_account_data_->bIsTradeApiConnected){
            break;
        }
    }

    ConnectToMarketDataApi();
    // save instrument info
    SaveInstrumentInfo();

    PrintNewMessage("ctp tick data start finish.");

}

void MyMainThread::PrintNewMessage(string message){
    printf("%s \n", message.c_str());
}

void MyMainThread::InitDataPointers(){
    my_account_data_ = new MyAccountDataStruct;
}

void MyMainThread::ReadAcocuntSettingFile(){

    ifstream v_inFile("account_setting.txt", ios::in);

    if (!v_inFile){
        PrintNewMessage("账户信息配置文件不存在");
        return;
    }
    v_inFile >> my_account_data_->investorID
            >> my_account_data_->password
            >> my_account_data_->brokerID
            >> my_account_data_->marketDataFrontAddr
            >> my_account_data_->tradeFrontAddr
            >> my_account_data_->appID
            >> my_account_data_->authCode;
    v_inFile.close();

}

void MyMainThread::ReadMysqlSettingFile(){
}

bool MyMainThread::ConnectToMysql(){
    return true;
}

bool MyMainThread::CreatMysqlTable(){
    return true;
}

void MyMainThread::ConnectToTrade_api(){
    //交易api对象初始化
    my_account_data_->pTradeApi = CThostFtdcTraderApi::CreateFtdcTraderApi();

    //创建spi对象
    MyCTPTradeSpi *my_ctp_trade_spi = new MyCTPTradeSpi;
    //将任务队列指针传给spi，由spi处理回调事务
    my_ctp_trade_spi->my_account_data = my_account_data_;

    //将spi注册给api
    my_account_data_->pTradeApi->RegisterSpi(my_ctp_trade_spi);
    my_account_data_->pTradeApi->SubscribePublicTopic(THOST_TERT_QUICK);
    my_account_data_->pTradeApi->SubscribePrivateTopic(THOST_TERT_QUICK);
    my_account_data_->pTradeApi->RegisterFront(my_account_data_->tradeFrontAddr);
    //自动创建线程，创建后调用spi中的OnFrontConnect函数
    my_account_data_->pTradeApi->Init();
}

void MyMainThread::ConnectToMarketDataApi(){
}

void MyMainThread::SaveInstrumentInfo(){
}

void MyMainThread::CreateTaskProcessThread(){}