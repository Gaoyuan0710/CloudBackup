find . -regextype posix-egrep -regex ".*\.(cpp|h|hpp)" | xargs perl -pi -e 's|\bepollfd\b|epoll_fd_|g'

find . -regextype posix-egrep -regex ".*\.(cpp|h|hpp)" | xargs perl -pi -e 's|\bsocketfd\b|socket_fd_|g'

find . -regextype posix-egrep -regex ".*\.(cpp|h|hpp)" | xargs perl -pi -e 's|\bServerIP\b|server_ip_|g'

find . -regextype posix-egrep -regex ".*\.(cpp|h|hpp)" | xargs perl -pi -e 's|\bPort\b|server_port_|g'

find . -regextype posix-egrep -regex ".*\.(cpp|h|hpp)" | xargs perl -pi -e 's|\bSetNonBlocking\b|setNonBlock|g'

find . -regextype posix-egrep -regex ".*\.(cpp|h|hpp)" | xargs perl -pi -e 's|\baddfd\b|AddFd|g'

find . -regextype posix-egrep -regex ".*\.(cpp|h|hpp)" | xargs perl -pi -e 's|\bSetEpollWait\b|setEpollFd|g'

find . -regextype posix-egrep -regex ".*\.(cpp|h|hpp)" | xargs perl -pi -e 's|\bEpollThreadID\b|epoll_thread_id|g'

find . -regextype posix-egrep -regex ".*\.(cpp|h|hpp)" | xargs perl -pi -e 's|\bManageServerIp\b|manage_server_ip|g'

find . -regextype posix-egrep -regex ".*\.(cpp|h|hpp)" | xargs perl -pi -e 's|\bManageServerPort\b|manage_server_port|g'

find . -regextype posix-egrep -regex ".*\.(cpp|h|hpp)" | xargs perl -pi -e 's|\bWorkPort\b|work_port|g'

find . -regextype posix-egrep -regex ".*\.(cpp|h|hpp)" | xargs perl -pi -e 's|\bWorkIp\b|work_ip|g'

find . -regextype posix-egrep -regex ".*\.(cpp|h|hpp)" | xargs perl -pi -e 's|\bUserName\b|user_name|g'

find . -regextype posix-egrep -regex ".*\.(cpp|h|hpp)" | xargs perl -pi -e 's|\bPasswd\b|passwd|g'

find . -regextype posix-egrep -regex ".*\.(cpp|h|hpp)" | xargs perl -pi -e 's|\brootFolder\b|root_folder|g'

find . -regextype posix-egrep -regex ".*\.(cpp|h|hpp)" | xargs perl -pi -e 's|\bm_maxSize\b|max_size_|g'

find . -regextype posix-egrep -regex ".*\.(cpp|h|hpp)" | xargs perl -pi -e 's|\bm_stop\b|stop_flag_|g'

find . -regextype posix-egrep -regex ".*\.(cpp|h|hpp)" | xargs perl -pi -e 's|\bm_notEmpty\b|not_empty_condition_|g'

find . -regextype posix-egrep -regex ".*\.(cpp|h|hpp)" | xargs perl -pi -e 's|\bm_notFull\b|not_full_condition_|g'

find . -regextype posix-egrep -regex ".*\.(cpp|h|hpp)" | xargs perl -pi -e 's|\bm_queue\b|queue_list_|g'

find . -regextype posix-egrep -regex ".*\.(cpp|h|hpp)" | xargs perl -pi -e 's|\bm_mutex\b|mutex_|g'

find . -regextype posix-egrep -regex ".*\.(cpp|h|hpp)" | xargs perl -pi -e 's|\bnumber_Threads\b|thread_numberi_|g'

find . -regextype posix-egrep -regex ".*\.(cpp|h|hpp)" | xargs perl -pi -e 's|\bm_running\b|running_flag_|g'

find . -regextype posix-egrep -regex ".*\.(cpp|h|hpp)" | xargs perl -pi -e 's|\bm_flag\b|once_flag_|g'





