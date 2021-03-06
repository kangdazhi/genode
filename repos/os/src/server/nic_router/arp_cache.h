/*
* \brief  Cache for received ARP information
* \author Martin Stein
* \date   2016-08-19
*/

/*
* Copyright (C) 2016 Genode Labs GmbH
*
* This file is part of the Genode OS framework, which is distributed
* under the terms of the GNU General Public License version 2.
*/

#ifndef _ARP_CACHE_H_
#define _ARP_CACHE_H_

/* Genode includes */
#include <net/ipv4.h>
#include <net/ethernet.h>
#include <util/avl_tree.h>
#include <util/volatile_object.h>

namespace Net {

	class Arp_cache;
	class Arp_cache_entry;
	using Arp_cache_entry_slot = Genode::Lazy_volatile_object<Arp_cache_entry>;
}


class Net::Arp_cache_entry : public Genode::Avl_node<Arp_cache_entry>
{
	private:

		Ipv4_address const _ip;
		Mac_address  const _mac;

		bool _higher(Ipv4_address const &ip) const;

	public:

		Arp_cache_entry(Ipv4_address const &ip, Mac_address const &mac);

		Arp_cache_entry const &find_by_ip(Ipv4_address const &ip) const;


		/**************
		 ** Avl_node **
		 **************/

		bool higher(Arp_cache_entry *entry) { return _higher(entry->_ip); }


		/***************
		 ** Accessors **
		 ***************/

		Mac_address const &mac() const { return _mac; }
};


class Net::Arp_cache : public Genode::Avl_tree<Arp_cache_entry>
{
	private:

		enum {
			ENTRIES_SIZE  = 1024 * sizeof(Genode::addr_t),
			NR_OF_ENTRIES = ENTRIES_SIZE / sizeof(Arp_cache_entry),
		};

		Arp_cache_entry_slot _entries[NR_OF_ENTRIES];
		bool                 _init = true;
		unsigned             _curr = 0;

	public:

		struct No_match : Genode::Exception { };

		void new_entry(Ipv4_address const &ip, Mac_address const &mac);

		Arp_cache_entry const &find_by_ip(Ipv4_address const &ip) const;
};

#endif /* _ARP_CACHE_H_ */
