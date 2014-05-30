#!/usr/bin/env python
#-*- coding:utf-8 –*-
from socket import *
import select
import locale

class TcpClient(object):
	def __init__(self,ip,port):
		self.ip = str(ip)
		self.port = int(port)

	def queryData(self,data):
		ret = ''
		self.sock = socket(AF_INET , SOCK_STREAM)
		self.sock.connect((self.ip,self.port))
		data = data.decode(locale.getpreferredencoding()).encode("utf-8")
		self.sock.send(data)
		while True:
			rs,ws,es = select.select([self.sock],[],[])
			if self.sock in rs:
				tmp = self.sock.recv(1024)
				if 0==len(tmp):
					break
				ret += tmp
		self.sock.close()
		return ret

class CsdnBlogClient(TcpClient):
	def __init__(self,ip,port):           
		TcpClient.__init__(self,ip,port)
	def do_query_keyword(self,keyword):   
		return self.do_exec_sql('select * from wordindex where keyword=\''+keyword+'\'')
	def do_exec_sql(self,sql):        
		return self.queryData('sql='+sql)
	def do_get_file(self,filename):       
		return self.queryData("file="+filename)

##################################################################

if __name__ == '__main__':
	blog  = CsdnBlogClient("127.0.0.1",8080)
	querydata = "指针"
	print blog.do_query_keyword(querydata)

	
	#print blog.do_get_file("1401372900.91.xml")
