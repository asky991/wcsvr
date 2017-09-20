#pragma once

#ifdef __linux
#include <unistd.h>
#include <signal.h>
#include <sys/param.h>
#include <sys/types.h>
#include <sys/stat.h>
#include<cstdlib> 
#include <cstring>
#include <string> 
#include <stdio.h>
#include <time.h>
#include<iostream>  
#include<algorithm> 

using namespace std;

const int BUF_LEN = 300;

void log_info(int argc, char* argv[]);
void linux_log(string msg);
int install_svr();
int uninstall_svr();
int exec_cmd(string cmd);
void init_daemon(void);
int init_proc();

int init_svr(int argc, char* argv[])
{
	if (0 == strcmp("-install", argv[1]) || 0 == strcmp("install", argv[1]))
	{
		install_svr();
	}
	else if (0 == strcmp("-uninstall", argv[1]) || 0 == strcmp("uninstall", argv[1]))
	{
		uninstall_svr();
	}
	else if (0 == strcmp("-run", argv[1]) || 0 == strcmp("run", argv[1]))
	{
		init_proc();
	}
	else
	{
		return -1;
	}

	//	FILE *fp; time_t t;
	//	init_daemon();//��ʼ��ΪDaemon
	//	while (1)//ÿ��һ������test.log��������״̬
	//	{
	//		sleep(5);//˯ ��һ����
	//		linux_log("");
	//	}
	return 0;
}


void log_info(int argc, char* argv[])
{
	for (int i = 0; i < argc; i++)
	{
		string str = "log_info:";
		str.append(argv[i]);
		linux_log(str);
	}
}


void init_daemon(void)
{
	int pid;
	int i;
	if (pid = fork())
	{
		linux_log("1-�Ǹ����̣�����������");
		exit(0);//�Ǹ����̣�����������
	}		
	else if (pid < 0)
	{
		linux_log("2-forkʧ�ܣ��˳�");
		exit(1);//forkʧ�ܣ��˳�
	}		
	//�ǵ�һ�ӽ��̣���̨����ִ��
	setsid();
	//��һ�ӽ��̳�Ϊ�µĻỰ�鳤�ͽ����鳤
	//��������� �˷���
	if (pid = fork())
	{
		linux_log("3-�ǵ�һ�ӽ��̣�������һ�ӽ���");
		exit(0);//�ǵ�һ�ӽ��̣�������һ�ӽ���
	}		
	else if (pid < 0)
	{
		linux_log("4-forkʧ�ܣ��˳�");
		exit(1);//forkʧ�ܣ��˳�
	}
		

	//�ǵڶ��ӽ��̣�����
	//�ڶ��ӽ��̲����ǻỰ�鳤
	for (i = 0; i < NOFILE; ++i)
	{
		//�رմ򿪵��ļ�������
		close(i);
	}


	chdir("/tmp"); //�ı乤��Ŀ¼��/tmp
	umask(0);//���� �ļ�������ģ

	return;

}

int init_proc()
{
	//FILE *fp; time_t t;
	init_daemon();//��ʼ��ΪDaemon
	while (1)//ÿ��һ������test.log��������״̬
	{
		sleep(5);//˯ ��һ����
		linux_log("");
	}
	return 0;
}


void linux_log(string msg)
{
	FILE *fp; time_t t;
	if ((fp = fopen("/tmp/test.log", "a")) >= 0)
	{
		if (msg.empty())
		{
			t = time(0);
			fprintf(fp, "Im here at %sn\n", asctime(localtime(&t)));			
		}
		else
		{
			fprintf(fp, "msg:%s\n", msg.c_str());
		}

		fclose(fp);
	}
}
int install_svr()
{
	uninstall_svr();
	exec_cmd("cp -rfu scripts/wcsvrd /etc/init.d/wcsvrd");
	exec_cmd("chmod 777 /etc/init.d/wcsvrd");
	exec_cmd("cp -rfu wcsvrd /usr/bin/wcsvrd ");
	exec_cmd("chmod 777 /etc/init.d/wcsvrd");
	exec_cmd("service wcsvrd start");
	return 0;
}

int uninstall_svr()
{
	exec_cmd("service wcsvrd stop");
	exec_cmd("rm -rf /etc/init.d/wcsvrd");
	exec_cmd("rm -rf /etc/init.d/wcsvrd ");
	
	return 0;
}

int exec_cmd(string cmd)
{
	char line[BUF_LEN];
	memset(line, 0, BUF_LEN);
	linux_log(cmd);
	FILE *fp;
	if ((fp = popen(cmd.c_str(), "r")) == NULL) {
		cout << "error" << endl;
		return -1;
	}
	while (fgets(line, sizeof(line) - 1, fp) != NULL) {
		//cout << line << endl;
		linux_log(line);
	}
	pclose(fp);

	return 0;
}


#endif // UNIX
