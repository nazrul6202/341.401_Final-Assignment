#pragma once
#include <iomanip>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>

using namespace std;


struct ip
{
	int b1;
	int b2;
	int b3;
	int b4;
};

class subnet
{
private:
	int hosts;
	ip netmask;
	ip first;
	ip last;
	int netmask_slash;

public:
	subnet(int hosts_);
	subnet(char input[]);
	subnet();

	void calc_netmask();
	int calc_hosts_real(){ return hosts-2;};
	int calc_hosts();
	void calc_ip_last();
	string print_net();

	void set_ip_first(ip first_);
	void set_ip_first(int b1, int b2, int b3, int b4);
	void set_ip_last(ip last_);
	void set_hosts(int hosts_);
	void set_netmask(int mask_b1, int mask_b2, int mask_b3, int mask_b4);
	void set_netmask_slash(int mask);

	int get_hosts(){ return hosts; };
	ip get_ip_first() { return first; };
	ip get_ip_last() { return last; };
	ip get_netmask(){ return netmask; };
	int get_netmask_slash(){ return netmask_slash;};

};

void sort_nets(subnet* sn, int number_subnets);

ip add_ip(ip ip_, int adder);
