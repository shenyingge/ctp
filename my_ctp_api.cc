#include "my/my_ctp_api.h"
#include <cstring>
#include <iostream>
#include <iomanip>

using namespace std;

void MyCTPTradeSpi::OnFrontConnected()
{

    m_investorID = string(my_account_data->investorID);
    PrintNewMessage("开始建立 " + m_investorID + " 交易Api...");
    ReqAuthenticate();
}

void MyCTPTradeSpi::ReqAuthenticate()
{

    CThostFtdcReqAuthenticateField v_req;
    memset(&v_req, 0, sizeof(v_req));
    strcpy(v_req.BrokerID, my_account_data->brokerID);
    strcpy(v_req.UserID, my_account_data->investorID);
    strcpy(v_req.AppID, my_account_data->appID);
    strcpy(v_req.AuthCode, my_account_data->authCode);

    int v_err = my_account_data->pTradeApi->ReqAuthenticate(&v_req, 0);

    if (v_err == 0)
    {
        PrintNewMessage("发送账号 " + m_investorID + " 穿透式认证请求成功！");
    }
}

void MyCTPTradeSpi::OnRspAuthenticate(CThostFtdcRspAuthenticateField *pRspAuthenticateField, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{

    if (bIsLast)
    {
        // 客户端认证失败
        if (IsErrorRspInfo(pRspInfo))
        {

            PrintNewMessage("账号 " + m_investorID + " 穿透式认证失败");
        }
        else
        {
            PrintNewMessage("账号 " + m_investorID + " 穿透式认证成功");
            ReqUserLogin();
        }
    }
}

void MyCTPTradeSpi::ReqUserLogin()
{
    CThostFtdcReqUserLoginField v_req;

    memset(&v_req, 0, sizeof(v_req));
    strcpy(v_req.BrokerID, my_account_data->brokerID);
    strcpy(v_req.UserID, my_account_data->investorID);
    strcpy(v_req.Password, my_account_data->password);

    int v_error = my_account_data->pTradeApi->ReqUserLogin(&v_req, 0);

    if (v_error == 0)
    {
        PrintNewMessage("发送账号 " + m_investorID + " 登录请求成功");
    }
    else
    {
        PrintNewMessage("发送账号 " + m_investorID + " 登录请求失败");
    }
}

void MyCTPTradeSpi::OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
    if (bIsLast)
    {
        // 登录失败
        if (IsErrorRspInfo(pRspInfo))
        {
            PrintNewMessage("账号 " + m_investorID + " 登录失败！");
            my_account_data->pTradeApi->Release();
        }
        else
        {
            PrintNewMessage("账号 " + m_investorID + " 登录成功，开始确认结算单！");
            ReqSettlementInfoConfirm();
        }
    }
}

void MyCTPTradeSpi::ReqSettlementInfoConfirm()
{

    CThostFtdcSettlementInfoConfirmField v_req;
    memset(&v_req, 0, sizeof(v_req));
    strcpy(v_req.BrokerID, my_account_data->brokerID);
    strcpy(v_req.InvestorID, my_account_data->investorID);

    int v_error = my_account_data->pTradeApi->ReqSettlementInfoConfirm(&v_req, 0);
    if (v_error == 0)
    {
        PrintNewMessage("发送账号 " + m_investorID + " 确认结算单结果请求成功");
    }
    else
    {
        PrintNewMessage("发送账号 " + m_investorID + " 确认结算单结果请求失败");
    }
}

void MyCTPTradeSpi::OnRspSettlementInfoConfirm(CThostFtdcSettlementInfoConfirmField *pSettlementInfoConfirm, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
    if (bIsLast && !IsErrorRspInfo(pRspInfo))
    {
        PrintNewMessage("账号 " + m_investorID + " 确认结算结果成功，开始查询合约信息");
        ReqQryInstrument();
    }
    else
    {
        PrintNewMessage("账号 " + m_investorID + " 确认结算结果失败");
    }
}

void MyCTPTradeSpi::ReqQryInstrument()
{
    CThostFtdcQryInstrumentField v_req;
    memset(&v_req, 0, sizeof(v_req));

    // 查询所有可用合约代码
    strcpy(v_req.InstrumentID, "");

    int v_error = my_account_data->pTradeApi->ReqQryInstrument(&v_req, 0);
    if (v_error == 0)
    {
        PrintNewMessage("账号 " + m_investorID + " 发送查询合约列表请求成功");
    }
    else
    {
        PrintNewMessage("账号 " + m_investorID + " 确认结算结果失败");
    }
}

void MyCTPTradeSpi::OnRspQryInstrument(CThostFtdcInstrumentField *pInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
    if (!IsErrorRspInfo(pRspInfo) && pInstrument != NULL)
    {
        string v_instrumentID = pInstrument->InstrumentID;

        // 只处理期货合约
        if (pInstrument->ProductClass == THOST_FTDC_PC_Futures && pInstrument->IsTrading)
        {

            // 初始化合约信息
            MyInstrumentInfoDataStruct *my_instrument_data = new MyInstrumentInfoDataStruct;
            my_instrument_data->instrumentId = pInstrument->InstrumentID;
            my_instrument_data->exchangeId = pInstrument->ExchangeID;
            my_instrument_data->volumeMultiplier = pInstrument->VolumeMultiple;
            my_instrument_data->longMarginRatio = pInstrument->LongMarginRatio;
            my_instrument_data->shortMarginRatio = pInstrument->ShortMarginRatio;
            my_instrument_data->priceTick = pInstrument->PriceTick;
            std::cout << "交易所代码： " << pInstrument->ExchangeID << std::endl;
            std::cout << "合约代码： " << pInstrument->InstrumentID << std::endl;
            std::cout << "合约在交易所的代码： " << pInstrument->ExchangeInstID << std::endl;
            std::cout << "执行价： " << pInstrument->StrikePrice << std::endl;
            std::cout << "到期日： " << pInstrument->EndDelivDate << std::endl;
            std::cout << "当前交易状态： " << pInstrument->IsTrading << std::endl;
            my_account_data->map_allAvailableInstrumentInfo[v_instrumentID] = my_instrument_data;
        }
    }
    if (bIsLast)
    {
        // my_account_data->bIsTradeApiConnected = true;
        PrintNewMessage(m_investorID + " 交易Api建立完成");
        ReqQryTradingAccount();
    }
}

void MyCTPTradeSpi::ReqQryTradingAccount()
{
    CThostFtdcQryTradingAccountField v_req;
    memset(&v_req, 0, sizeof(v_req));
    strcpy(v_req.BrokerID, my_account_data->brokerID);
    strcpy(v_req.InvestorID, my_account_data->investorID);
    std::this_thread::sleep_for(std::chrono::milliseconds(7000)); // 有时候需要停顿一会才能查询成功
    int rt = my_account_data->pTradeApi->ReqQryTradingAccount(&v_req, 0);
    if (rt == 0)
        std::cout << ">>>>>>发送投资者资金账户查询请求成功" << std::endl;
    else
        std::cerr << "--->>>发送投资者资金账户查询请求失败" << std::endl;
}

void MyCTPTradeSpi::OnRspQryTradingAccount(
    CThostFtdcTradingAccountField *pTradingAccount,
    CThostFtdcRspInfoField *pRspInfo,
    int nRequestID,
    bool bIsLast)
{
    if (!IsErrorRspInfo(pRspInfo))
    {
        std::cout << "=====查询投资者资金账户成功=====" << std::endl;
        std::cout << "投资者账号： " << pTradingAccount->AccountID << std::endl;
        std::cout << "可用资金： " << std::fixed << std::setprecision(2) << pTradingAccount->Available << std::endl;
        std::cout << "可取资金： " << std::fixed << std::setprecision(2) << pTradingAccount->WithdrawQuota << std::endl;
        std::cout << "当前保证金: " << pTradingAccount->CurrMargin << std::endl;
        std::cout << "平仓盈亏： " << pTradingAccount->CloseProfit << std::endl;
        // 请求查询投资者持仓
        reqQueryInvestorPosition();
    }
}

void MyCTPTradeSpi::reqQueryInvestorPosition()
{
    CThostFtdcQryInvestorPositionField v_req;
    memset(&v_req, 0, sizeof(v_req));
    strcpy(v_req.BrokerID, my_account_data->brokerID);
    strcpy(v_req.InvestorID, my_account_data->investorID);
    std::this_thread::sleep_for(std::chrono::milliseconds(7000)); // 有时候需要停顿一会才能查询成功
    int rt = my_account_data->pTradeApi->ReqQryInvestorPosition(&v_req, 0);
    if (rt == 0)
        std::cout << ">>>>>>发送投资者持仓查询请求成功" << std::endl;
    else
        std::cerr << "--->>>发送投资者持仓查询请求失败" << std::endl;
}

void MyCTPTradeSpi::OnRspQryInvestorPosition(
    CThostFtdcInvestorPositionField *pInvestorPosition,
    CThostFtdcRspInfoField *pRspInfo,
    int nRequestID,
    bool bIsLast)
{
    if (!IsErrorRspInfo(pRspInfo) && bIsLast){
        std::cout << "持仓: "<<endl;
        if (pInvestorPosition == nullptr){
            cout<<"没有持仓"<<endl;
            return;
        }
        cout<<pInvestorPosition->InstrumentID<<endl;
        cout<<pInvestorPosition->Position<<endl;
    }
}

void MyCTPTradeSpi::PrintNewMessage(string message)
{
    printf("%s \n", message.c_str());
}

bool MyCTPTradeSpi::IsErrorRspInfo(CThostFtdcRspInfoField *pRspInfo)
{
    bool bResult = pRspInfo && (pRspInfo->ErrorID != 0);
    if (bResult)
        PrintNewMessage("返回错误: " + to_string(pRspInfo->ErrorID) + " " + pRspInfo->ErrorMsg);
    return bResult;
}
