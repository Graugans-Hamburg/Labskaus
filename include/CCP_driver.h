#ifndef CCP_DRIVER_H
#define CCP_DRIVER_H


class CCP_driver
{
    public:
        CCP_driver();
        virtual ~CCP_driver();
        void SendCCPFrame();
        void AnalyzeCCPFrame();
    protected:
    private:
        bool CommunicationChannel_Active;
        bool Device_Available;
};

#endif // CCP_DRIVER_H
