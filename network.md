###################################
# Redis 网络分析
####################################


# socket建立
aeMain(...)<br>
	-> aeProcessEvents(...)<br>
		-> aeApiPoll(...)<br>

# accept处理
aeCreateFileEvent(server.el, server.ipfd[j], AE_READABLE, acceptTcpHandler, NULL); <br>
aeCreateFileEvent(...) # 处理读写事件<br>
	->acceptTcpHandler(...) # 如下<br>
		-> anetTcpAccept(...) # 如下<br>
			-> anetGenericAccept(...) # 如下<br>
				-> accept(...) #最终Linux Socket API<br>
	->acceptCommonHandler(...) <br>
		-> createClient(...)  #创建客户<br>
			-> anetNonBlock(...) # 设置非阻塞<br>
			-> anetEnableTcpNoDelay(...)<br>
				-> setsockopt(...) # 设置IPPROTO_TCP:TCP_NODELAY 非延迟<br>
			-> anetKeepAlive(...) # 设置心跳包<br>
				-> setsockopt(...) # SOL_SOCKET:SO_KEEPALIVE 开启心跳包<br>
				-> setsockopt(...) # 设置心跳包时间参数 IPPROTO_TCP:TCP_KEEPIDLE, IPPROTO_TCP:TCP_KEEPINTVL
								   # IPPROTO_TCP:TCP_KEEPCNT
			-> aeCreateFileEvent(...readQueryFromClient...)<br>
				-> readQueryFromClient(...)  # 读取客户端数据<br>
					-> read(...) # Linux API 读取函数<br>
					-> processInputBuffer(...) #处理读取数据<br>
						-> processInlineBuffer(...) # 单行命令处理<br>
						-> processMultibulkBuffer(...) <br>
						-> processCommand(...) <br>


