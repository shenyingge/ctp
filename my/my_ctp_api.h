#include "my_data_struct.h"

class MyCTPTradeSpi:public CThostFtdcTraderSpi
{
public:

    MyAccountDataStruct *my_account_data;

    //连接前台
    virtual void OnFrontConnected();
    //客户端认证响应
	virtual void OnRspAuthenticate(CThostFtdcRspAuthenticateField *pRspAuthenticateField, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	///登录请求响应
	virtual void OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	///投资者结算结果确认响应
	virtual void OnRspSettlementInfoConfirm(CThostFtdcSettlementInfoConfirmField *pSettlementInfoConfirm, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	///请求查询合约响应
	virtual void OnRspQryInstrument(CThostFtdcInstrumentField *pInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

    virtual void OnRspQryTradingAccount(CThostFtdcTradingAccountField *pTradingAccount, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

    virtual void OnRspQryInvestorPosition(CThostFtdcInvestorPositionField *pInvestorPosition, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
private:
    string m_investorID;

    void PrintNewMessage(string message);
    bool IsErrorRspInfo(CThostFtdcRspInfoField *pRspInfo);

    void ReqAuthenticate();
    void ReqUserLogin();
    void ReqSettlementInfoConfirm();
    void ReqQryInstrument();
    void ReqQryTradingAccount();
    void reqQueryInvestorPosition();
    // void
};