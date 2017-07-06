###################################
# Redis 网络分析
####################################

server.c
socket:
aeMain(...)
	-> aeProcessEvents(...)
		-> aeApiPoll(...)

accept:
aeCreateFileEvent(server.el, server.ipfd[j], AE_READABLE, acceptTcpHandler, NULL); 
aeCreateFileEvent(...) # 处理读写事件
	->acceptTcpHandler(...) # 如下
		-> anetTcpAccept(...) # 如下
			-> anetGenericAccept(...) # 如下
				-> accept(...) #最终Linux Socket API
	->acceptCommonHandler(...) 
		-> createClient(...)  #创建客户
			-> anetNonBlock(...) # 设置非阻塞
			-> anetEnableTcpNoDelay(...)
				-> setsockopt(...) # 设置IPPROTO_TCP:TCP_NODELAY 非延迟
			-> anetKeepAlive(...) # 设置心跳包
				-> setsockopt(...) # SOL_SOCKET:SO_KEEPALIVE 开启心跳包
				-> setsockopt(...) # 设置心跳包时间参数 IPPROTO_TCP:TCP_KEEPIDLE, IPPROTO_TCP:TCP_KEEPINTVL
								   # IPPROTO_TCP:TCP_KEEPCNT
			-> aeCreateFileEvent(...readQueryFromClient...)
				-> readQueryFromClient(...)  # 读取客户端数据
					-> read(...) # Linux API 读取函数
					-> processInputBuffer(...) #处理读取数据
						-> processInlineBuffer(...) # 单行命令处理
						-> processMultibulkBuffer(...) 
						-> processCommand(...) 


