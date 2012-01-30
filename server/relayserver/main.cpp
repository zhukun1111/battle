
int main(int argc, char** argv)
{
	IWMNetWork*		poNetwork = NULL;
	CWMCoder*		poCoder = new CWMCoder;
	CWMSender*		poSender = new CWMSender(poCoder, poNetwork);

	CNetProcessor*	poNetProcessor = new CNetProcessor(poNetwork, poCoder, poSender);

	WM_NetListen<CClientLink>(poNetProcessor, poNetwork);

	while(true)
	{
		if(false == poNetwork->ProcessMsg(1000))
		{
			sleep(1);
		}
	}
}