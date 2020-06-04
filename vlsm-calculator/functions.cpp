#pragma once
#include "functions.h"


subnet::subnet(int hosts_)
{
	if(hosts_ >=0 && hosts_ <= this->calc_hosts())
	{
		set_hosts(hosts_);
		calc_netmask();
	}
}

subnet::subnet(char input[])
{
	char byte1[3] = {0};
	char byte2[3] = {0};
	char byte3[3] = {0};
	char byte4[3] = {0};
	char mask[3] = {0};
	int i = 0;
	while (input[i] != '.')
	{
		static int j=0;
		byte1[j] = input[i];
		j++;
		i++;
	}
	i++;

	while (input[i] != '.')
	{
		static int j=0;
		byte2[j] = input[i];
		j++;
		i++;
	}

	i++;
	while (input[i] != '.')
	{
		static int j=0;
		byte3[j] = input[i];
		j++;
		i++;
	}

	i++;
	while (input[i] != '/')
	{
		static int j=0;
		byte4[j] = input[i];
		j++;
		i++;
	}

	i++;
	while (input[i] != '\0')
	{
		static int j=0;
		mask[j] = input[i];
		j++;
		i++;
	}

	this->set_ip_first(atoi(byte1), atoi(byte2), atoi(byte3), atoi(byte4));
	this->set_netmask_slash(atoi(mask));
	this->set_hosts(this->calc_hosts());
	this->calc_netmask();
	this->set_ip_first(this->get_ip_first()); //
	this->set_ip_last(add_ip(this->get_ip_first(), this->get_hosts()-1));
}

subnet::subnet()
{

}

void subnet::set_ip_first(ip first_)
{
	this->first.b1 = first_.b1 & this->netmask.b1;
	this->first.b2 = first_.b2 & this->netmask.b2;
	this->first.b3 = first_.b3 & this->netmask.b3;
	this->first.b4 = first_.b4 & this->netmask.b4;
}

void subnet::set_ip_first(int b1, int b2, int b3, int b4)
{
	this->first.b1 = b1;
	this->first.b2 = b2;
	this->first.b3 = b3;
	this->first.b4 = b4;
}

void subnet::set_ip_last(ip last_)
{
	this->last = last_;
}

void subnet::set_hosts(int hosts_)
{
	hosts = hosts_;
}

void subnet::set_netmask(int mask_b1, int mask_b2, int mask_b3, int mask_b4)
{
	if(mask_b1 >= 0 && mask_b1 <256)
	{
		netmask.b1 = mask_b1;
	}

	if(mask_b2 >= 0 && mask_b2 <256)
	{
		netmask.b2 = mask_b2;
	}

	if(mask_b3 >= 0 && mask_b3 <256)
	{
		netmask.b3 = mask_b3;
	}

	if(mask_b4 >= 0 && mask_b4 <256)
	{
		netmask.b4 = mask_b4;
	}
}

void subnet::set_netmask_slash(int mask)
{
	if(mask >=0 && mask <= 32)
	{
		netmask_slash = mask;
	}
}

int subnet::calc_hosts()
{
	return 1 << (32-get_netmask_slash());
}

void subnet::calc_netmask()
{
	int power = 0;
	int divided;
	int rest;
	for(power; (1 << power) < get_hosts(); power++);
	set_netmask_slash(32-power);

	divided = (32-power)/8;
	rest = (32-power)%8;

	switch(divided)
	{
	case 4:
		set_netmask(255, 255, 255, 255);
		break;

	case 3:
		set_netmask(255, 255, 255, (256-(1<<(8-rest))));
		break;

	case 2:
		set_netmask(255, 255, (256 - (1 << (8-rest))), 0);
		break;

	case 1:
		set_netmask(255, (256 - (1 << (8-rest))), 0, 0);
		break;

	case 0:
		set_netmask((256 - (1 << (8-rest))), 0, 0, 0);
		break;

	}
}

void subnet::calc_ip_last()
{
	this->get_hosts() / 256;
	this->get_hosts() % 256;
}

string subnet::print_net()
{
	char output[4000];
	snprintf(output, 4000, "Network Address: %i.%i.%i.%i/%i\nBroadcast Address: %i.%i.%i.%i\nNetwork Mask: %i.%i.%i.%i\nHosts: %i\nUsable Hosts: %i\n\n\n",
		this->get_ip_first().b1, this->get_ip_first().b2, this->get_ip_first().b3, this->get_ip_first().b4, this->get_netmask_slash(),
		this->get_ip_last().b1, this->get_ip_last().b2, this->get_ip_last().b3, this->get_ip_last().b4,
		this->get_netmask().b1,
		this->get_netmask().b2, this->get_netmask().b3, this->get_netmask().b4,
		this->get_hosts(),	this->calc_hosts_real());
	return output;

}


void sort_nets(subnet* sn, int number_subnets)
{
	subnet temp();
	int i, j;
	for (i = 0; i < number_subnets -1; ++i)
	{
		for (j = 0; j < number_subnets - i - 1; ++j)
		{
			if (sn[j].get_hosts() < sn[j + 1].get_hosts())
			{
				subnet tmp = sn[j];
				sn[j] = sn[j + 1];
				sn[j + 1] = tmp;
			}
		}
	}
}

ip add_ip(ip ip_, int adder)
{
	ip_.b4 = ip_.b4+adder;
	ip_.b3 = ip_.b3+(ip_.b4/256);
	ip_.b2 = ip_.b2+(ip_.b3/256);
	ip_.b1 = ip_.b1+(ip_.b2/256);

	ip_.b4 = ip_.b4%256;
	ip_.b3 = ip_.b3%256;
	ip_.b2 = ip_.b2%256;
	ip_.b1 = ip_.b1%256;

	return ip_;

}
