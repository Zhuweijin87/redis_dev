###################################
# Redis 网络分析
####################################

1. aeProcessEvents 处理客户端事件\n
	|--> aeApiPoll 客户请求处理的封装层\n
		|--> epoll_wait(linux API), select(linux API), kqueue(FreeBSD API) 底层调用系统API\n
		|--> { 如果是读事件：包括新客户连接，客户端发送数据:\n
				|-> 
