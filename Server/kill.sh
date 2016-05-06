ps -ef|grep python|grep -v grep|awk '{print $2}'|xargs kill -9
ps -ef|grep manageServer|grep -v grep|awk '{print $2}'|xargs kill -9
ps -ef|grep workServer|grep -v grep|awk '{print $2}'|xargs kill -9

#cd ManageServer
#./manageServer & > /dev/null 
#cd ../BalanceServer
#python balanceServer.py & > /dev/null 
#cd ../WorksServer
#./workServer & > /dev/null 
