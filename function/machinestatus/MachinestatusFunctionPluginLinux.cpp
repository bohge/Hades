#include<iostream>
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>  
#include <ctype.h>
#include <time.h>
#include <sys/time.h> 
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "serialize/ISerializeSystem.h"
#include "script/IScriptSystem.h"
#include "MachinestatusFunctionPluginLinux.h"
#include "AllRegister.hpp"
using namespace std;

#define _LINE_LENGTH 300 
#define __DEBUG__ 1  
#define CK_TIME 1  
#define MAX_BUFF_SIZE 256

#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif


namespace hmsfp
{	

	RTTI_TYPE_IMPLEMENT(MachinestatusFunctionPlugin);
	SYSTEM_SINGLETON_IMPLEMENT(MachinestatusFunctionPlugin);

string netcardname;
struct timeval t_start; 

typedef struct PACKED_mem         //定义一个mem occupy的结构体
{
	char name[20];      //定义一个char类型的数组名name有20个元素
	unsigned long total;
	char name2[20];
	unsigned long free;
}MEM_OCCUPY;
	
struct net_stat {
    const char *dev;
    int up;
    long long last_read_recv, last_read_trans; //last read total num
    long long recv, trans; //real total num
    double recv_speed, trans_speed;
};

/* network interface stuff */
static struct net_stat netstats[16]={0};


	
struct net_stat *get_net_stat(const char *dev)
{
    unsigned int i=0;
    if (!dev) {
        return 0;
    }
    /* find interface stat */
    for (i = 0; i < 16; i++) {
        if (netstats[i].dev && strcmp(netstats[i].dev, dev) == 0) {
            return &netstats[i];
        }
    }
    /* wasn't found? add it */
    if (i == 16) {
        for (i = 0; i < 16; i++) {
            if (netstats[i].dev == 0) {
                netstats[i].dev = strndup(dev, MAX_BUFF_SIZE);
                return &netstats[i];
            }
        }
    }
    fprintf(stderr, "too many interfaces used (limit is 16)");
    return 0;
}

void clear_net_stats(void)
{
    memset(netstats, 0, sizeof(netstats));
}

string getFirstNetCardName()
{
	FILE *net_dev_fp;
    int i=0;
    char buf[256]={0};
	char *s=NULL, *p=NULL;
	string res = "";
    /* open file and ignore first two lines */

    if (!(net_dev_fp = fopen("/proc/net/dev", "r"))) {
        fprintf(stderr, "fopen failed.\n");
        clear_net_stats();
        return res;
    }

	fgets(buf, 255, net_dev_fp);
	fgets(buf, 255, net_dev_fp);
	if (fgets(buf, 255, net_dev_fp) == NULL) {
		fclose(net_dev_fp);
        return res;
    }
	p = buf;

	while (isspace((int) *p)) {
        p++;
    }

	s = p;

    //Skip Network Interface Name
    while (*p && *p != ':') {
        p++;
    }
    if (*p == '\0') {
        return "";
    }
    *p = '\0';
	p++;
	
	res = s;
	fclose(net_dev_fp);
	return res;
}

void update_net_stats(const char* dev, double delta)
{
    FILE *net_dev_fp;
    // FIXME: arbitrary size chosen to keep code simple.
    int i=0;
    char buf[256]={0};
    /* open file and ignore first two lines */
    if (!(net_dev_fp = fopen("/proc/net/dev", "r"))) {
        fprintf(stderr, "fopen failed.\n");
        clear_net_stats();
        return;
    }
    fgets(buf, 255, net_dev_fp);    /* garbage */
	//cout << buf << endl;
    //fgets(buf, 255, net_dev_fp);    /* garbage (field names) */
    /* read each interface */
    for (i = 0; i < 16; i++) {
        struct net_stat *ns=NULL;
        char *s=NULL, *p=NULL;
        unsigned long long r=0, t=0;
        unsigned long long last_recv=0, last_trans=0;
        if (fgets(buf, 255, net_dev_fp) == NULL) {
            //File EOF
            break;
        }
        //Skip Space
        p = buf;
        while (isspace((int) *p)) {
            p++;
        }
        /*s: network interface name*/
        s = p;
        //Skip Network Interface Name
        while (*p && *p != ':') {
            p++;
        }
        if (*p == '\0') {
            continue;
        }
        *p = '\0';
        /*p: reveive bytes*/
        p++;
        //Judge Network Interface or Not?
        if(strcmp(s, dev) != 0)
            continue;
        //Get struct net_stat
        ns = get_net_stat(s);
        ns->up = 1;
        last_recv = ns->recv;
        last_trans = ns->trans;
        /* bytes packets errs drop fifo frame compressed multicast|bytes ... */
		//cout << p << endl;
		//cout << "11111111111111" << endl;
        sscanf(p, "%lld  %*d     %*d  %*d  %*d  %*d   %*d        %*d       %lld",
            &r, &t);
		//cout << r << endl;
		//cout << ns->last_read_recv << endl;
        /* if recv or trans is less than last time, an overflow happened */
        if (r < ns->last_read_recv) {
            last_recv = 0;
        } else {
            ns->recv += (r - ns->last_read_recv);
        }
        ns->last_read_recv = r;
        if (t < ns->last_read_trans) {
            last_trans = 0;
        } else {
            ns->trans += (t - ns->last_read_trans);
        }
        ns->last_read_trans = t;
        /* calculate speeds */
        if(last_recv == 0)
            ns->recv_speed = 0;
        else
            ns->recv_speed = (ns->recv - last_recv) / delta;
        if(last_trans == 0)
            ns->trans_speed = 0;
        else
            ns->trans_speed = (ns->trans - last_trans) / delta;
        /*
        //First Time Run, Or Time Overflow
        if(current_time == 0)
        {
            ns->recv_speed = 0;
            ns->trans_speed = 0;
        }
        */
        //Find Network Interface, And Work Over
        break;
    }
    fclose(net_dev_fp);
}


	int GetCpuBusypercentageByPID(int pid){
		bool ret = false;
		float cpu = 0;
		size_t mem = 0;
		char cmdline[100];
		sprintf(cmdline, "ps -o %%cpu,rss,%%mem,pid,tid -mp %d", pid);
		FILE *file;
		file = popen(cmdline, "r");
		if (file == NULL)
		{
			printf("file == NULL\n");
			return false;
		}

		char line[_LINE_LENGTH];
		float l_cpuPrec = 0;
		int l_mem = 0;
		float l_memPrec = 0;
		int l_pid = 0;
		int l_tid = 0;
		if (fgets(line, _LINE_LENGTH, file) != NULL)
		{
			//  printf("1st line:%s",line); 
			if (fgets(line, _LINE_LENGTH, file) != NULL)
			{
				//      printf("2nd line:%s",line); 
				sscanf(line, "%f %d %f %d -", &l_cpuPrec, &l_mem, &l_memPrec, &l_pid);
				cpu = l_cpuPrec;
				mem = l_mem / 1024;
			}
			else
				printf("PID not exist\n");
		}
		else
			printf("Command or Parameter wrong\n");
		pclose(file);
		return l_mem;

	}


	float MachinestatusFunctionPlugin::GetCpuBusypercentage(){
		FILE *fp;
		char buf[128];
		char cpu[5];
		long long user, nice, sys, idle, iowait, irq, softirq;

		long long all1, all2, idle1, idle2;
		float usage;
		struct timeval now; 
		gettimeofday(&now, NULL);
		double nowcputime = ((double)now.tv_sec)*1000+(double)now.tv_usec/1000; 
		
			fp = fopen("/proc/stat", "r");
			if (fp == NULL)
			{
				perror("fopen:");
				exit(0);
			}


			fgets(buf, sizeof(buf), fp);
#if __DEBUG__  
			//printf("buf=%s", buf);
#endif  
			sscanf(buf, "%s%lld%lld%lld%lld%lld%lld%lld", cpu, &user, &nice, &sys, &idle, &iowait, &irq, &softirq);
			/*
			#if __DEBUG__
			//printf("%s,%lld,%lld,%lld,%lld,%lld,%lld,%lld\n",cpu,user,nice,sys,idle,iowait,irq,softirq);
			#endif
			*/
			all2 = user + nice + sys + idle + iowait + irq + softirq;
			idle2 = idle;

			all1 = prekernelTime + preuserTime;
			idle1 = preidleTime;
			
			
			usage = (float)(all2 - all1 - (idle2 - idle1)) / (all2 - all1) * 100;

			/*cout << (nowcputime - lastcputime) << endl;
			printf("all1=%lld\n", all1);
			printf("all2=%lld\n", all2);
			printf("all-lld=%ld\n", all2 - all1);
			printf("all-int=%d\n", (int)(all2 - all1));
			printf("all-float=%f\n", (float)(all2 - all1));
			printf("ilde=%d\n", all2 - all1 - (idle2 - idle1));
			printf("cpu use = %.2f\%\n", usage);
			printf("=======================\n");*/

			lastcputime = nowcputime;
			prekernelTime = nice + sys + idle + iowait + irq + softirq;
			preuserTime = user;
			preidleTime = idle;
			
			fclose(fp);

			return usage;
		//}

	}

	int MachinestatusFunctionPlugin::GetFreePhysicalmemory(){

		FILE *fd;
		int n;
		char buff[256];
		MEM_OCCUPY m;

		fd = fopen("/proc/meminfo", "r");

		fgets(buff, sizeof(buff), fd);
		sscanf(buff, "%s %lu", m.name, &m.total);

		fgets(buff, sizeof(buff), fd); //从fd文件中读取长度为buff的字符串再存到起始地址为buff这个空间里 
		sscanf(buff, "%s %lu", m.name2, &m.free);

		fclose(fd);     //关闭文件fd
		return m.free;
	}

	int MachinestatusFunctionPlugin::GetTotalPhysicalmemory(){

		FILE *fd;
		int n;
		char buff[256];
		MEM_OCCUPY m;

		fd = fopen("/proc/meminfo", "r");

		fgets(buff, sizeof(buff), fd);
		sscanf(buff, "%s %lu %s", m.name, &m.total, m.name2);

		fgets(buff, sizeof(buff), fd); //从fd文件中读取长度为buff的字符串再存到起始地址为buff这个空间里 
		sscanf(buff, "%s %lu", m.name2, &m.free);

		fclose(fd);     //关闭文件fd
		return m.total;
	}

	int MachinestatusFunctionPlugin::GetTotalVirtualmemory(){

		FILE *fd;
		int n;
		char buff[256];
		MEM_OCCUPY m;

		fd = fopen("/proc/meminfo", "r");

		fgets(buff, sizeof(buff), fd);
		fgets(buff, sizeof(buff), fd);
		fgets(buff, sizeof(buff), fd);
		sscanf(buff, "%s %lu %s", m.name, &m.total, m.name2);

		fgets(buff, sizeof(buff), fd); //从fd文件中读取长度为buff的字符串再存到起始地址为buff这个空间里 
		sscanf(buff, "%s %lu", m.name2, &m.free);

		fclose(fd);     //关闭文件fd
		return m.total;
	}

	int MachinestatusFunctionPlugin::GetFreeVirtualmemory(){

		return 0;
	}

	float MachinestatusFunctionPlugin::GetNetcardReceiveSpeed(){
		struct net_stat* ns = NULL;
		struct timeval now; 
		float res = 0;
		
		gettimeofday(&now, NULL);
		double nownetcardrectime = ((double)now.tv_sec)*1000+(double)now.tv_usec/1000; 
		double delta_time = (nownetcardrectime - lastnetcardrectime) /1000 ;	
		
		update_net_stats(netcardname.c_str(), delta_time);
		ns = get_net_stat(netcardname.c_str());

		res = ns->recv_speed/1024;
		lastnetcardsendtime = nownetcardrectime;
		lastnetcardrectime = nownetcardrectime;
		return res;
	}
	
	float MachinestatusFunctionPlugin::GetNetcardSendSpeed(){
		struct net_stat* ns = NULL;
		struct timeval now; 
		float res = 0;
		
		gettimeofday(&now, NULL);
		double nownetcardrectime = ((double)now.tv_sec)*1000+(double)now.tv_usec/1000; 
		double delta_time = (nownetcardrectime - lastnetcardsendtime) /1000 ;	
		
		update_net_stats(netcardname.c_str(), delta_time);
		ns = get_net_stat(netcardname.c_str());

		res = ns->trans_speed / 1024;
		lastnetcardsendtime = nownetcardrectime;
		lastnetcardrectime = nownetcardrectime;
		return res;
	}
	
	
	void MachinestatusFunctionPlugin::Initialize(){
		
		hc::IScriptSystem* scriptsystem = hc::IScriptSystem::Instance();
		Register_All("machinestatus", *scriptsystem);

		netcardname = getFirstNetCardName();
		
		gettimeofday(&t_start, NULL);
		lastnetcardrectime = ((double)t_start.tv_sec)*1000+(double)t_start.tv_usec/1000; 
	
		lastnetcardrectime = lastnetcardrectime;
		lastnetcardsendtime = lastnetcardrectime;
		update_net_stats(netcardname.c_str(), lastnetcardrectime);
		
		gettimeofday(&t_start, NULL);
		lastcputime = ((double)t_start.tv_sec)*1000+(double)t_start.tv_usec/1000; 
		
		FILE *fp;
		char buf[128];
		char cpu[5];
		long long user, nice, sys, idle, iowait, irq, softirq;

		float usage;

		fp = fopen("/proc/stat", "r");
		if (fp == NULL)
		{
			perror("fopen:");
			exit(0);
		}

		fgets(buf, sizeof(buf), fp);
#if __DEBUG__  
		//printf("buf=%s", buf);
#endif  
		sscanf(buf, "%s%lld%lld%lld%lld%lld%lld%lld", cpu, &user, &nice, &sys, &idle, &iowait, &irq, &softirq);
		/*
		#if __DEBUG__
		//printf("%s,%d,%d,%d,%d,%d,%d,%d\n",cpu,user,nice,sys,idle,iowait,irq,softirq);
		#endif
		*/
		prekernelTime = nice + sys + idle + iowait + irq + softirq;
		preuserTime = user;
		preidleTime = idle;
		fclose(fp);
	}
	
	MachinestatusFunctionPlugin::MachinestatusFunctionPlugin()
	{	
		SYSTEM_SINGLETON_INITIALIZE
	}
	//---------------------------------------------------------------------------------------------------------
	MachinestatusFunctionPlugin::~MachinestatusFunctionPlugin()
	{
		SYSTEM_SINGLETON_DESTROY
	}
	//---------------------------------------------------------------------------------------------------------
	void MachinestatusFunctionPlugin::Update()
	{

	}
	//---------------------------------------------------------------------------------------------------------
	void MachinestatusFunctionPlugin::Exit()
	{

	}
}