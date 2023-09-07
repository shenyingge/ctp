#include "my_task_process_thread.h"

class MyMainThread
{

    public:

        void StartMainThread();

    private:
        MyAccountDataStruct *my_account_data_;

        void PrintNewMessage(string message);
        void InitDataPointers();
        void ReadAcocuntSettingFile();
        void ReadMysqlSettingFile();
        bool ConnectToMysql();
        bool CreatMysqlTable();
        void ConnectToTrade_api();
        void ConnectToMarketDataApi();
        void SaveInstrumentInfo();
        void CreateTaskProcessThread();

};