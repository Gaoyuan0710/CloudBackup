cd ManageServer
./manageServer  & > /dev/null
sleep 1
cd ../BalanceServer
python balanceServer.py  & > /dev/null 
sleep 1
cd ../WorksServer
./workServer  > /dev/null 
