find . -regextype posix-egrep -regex ".*\.(cpp|h|hpp)" | xargs perl -pi -e 's|\bepollfd\b|epoll_fd_|g'

find . -regextype posix-egrep -regex ".*\.(cpp|h|hpp)" | xargs perl -pi -e 's|\bsocketfd\b|socket_fd_|g'

find . -regextype posix-egrep -regex ".*\.(cpp|h|hpp)" | xargs perl -pi -e 's|\bServerIP\b|server_ip_|g'

find . -regextype posix-egrep -regex ".*\.(cpp|h|hpp)" | xargs perl -pi -e 's|\bPort\b|server_port_|g'

find . -regextype posix-egrep -regex ".*\.(cpp|h|hpp)" | xargs perl -pi -e 's|\bSetNonBlocking\b|setNonBlock|g'

find . -regextype posix-egrep -regex ".*\.(cpp|h|hpp)" | xargs perl -pi -e 's|\baddfd\b|AddFd|g'

find . -regextype posix-egrep -regex ".*\.(cpp|h|hpp)" | xargs perl -pi -e 's|\bSetEpollWait\b|setEpollFd|g'
