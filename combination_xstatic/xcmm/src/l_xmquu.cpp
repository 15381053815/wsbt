#include "l_xmquu.h"
#include "am_xarith.h"

lxquu::lxquu()
{
	am_parem pm={0};
	pm.umode=amdm_nomem;
	pm.type=amot_doubly_list;
	pm.unitsize=sizeof(lqmsg);

	_lstin=am_createobject_m(&pm);
	_lstot=am_createobject_m(&pm);
}

lxquu::~lxquu()
{
	am_destryobject(_lstin,
		amot_doubly_list);
	am_destryobject(_lstot,
		amot_doubly_list);
}

void	lxquu::lqpshinmsg(lqmsg &mn, bool brep)
{
	_lkin.lock();
	amx_list_insert_cpy(_lstin,
		&mn,sizeof(mn),0);
	_lkin.unlock();
}

int		lxquu::lqgetinmsg(lqmsg &mn, bool bdel)
{
	_lkin.lock();
	am_node *node=amx_list_frst(_lstin);
	if (node){
		mn=*(lqmsg*)node->data;
		if (bdel)amx_list_pop(_lstin);
	}
	_lkin.unlock();
	return (int)node;
}

void	lxquu::lqclrinmsg()
{
	_lkin.lock();
	amx_list_clear(_lstin);
	_lkin.unlock();
}

void	lxquu::lqpshotmsg(lqmsg &mn, bool brep)
{
	_lkot.lock();
	amx_list_insert_cpy(_lstot,
		&mn,sizeof(mn),0);
	_lkot.unlock();
}

int		lxquu::lqgetotmsg(lqmsg &mn, bool bdel)
{
	_lkot.lock();
	am_node *node=amx_list_frst(_lstot);
	if (node){
		mn=*(lqmsg*)node->data;
		if (bdel)amx_list_pop(_lstot);
	}
	_lkot.unlock();
	return (int)node;
}

void	lxquu::lqclrotmsg()
{
	_lkot.lock();
	amx_list_clear(_lstot);
	_lkot.unlock();
}

int		lxquu::lqgetinmsgsize()
{
	int sz = 0;
	_lkin.lock();
	sz = amx_list_count(_lstin);
	_lkin.unlock();
	return sz;
}

int		lxquu::lqgetotmsgsize()
{
	int sz = 0;
	_lkot.lock();
	sz =amx_list_count(_lstot);
	_lkot.unlock();
	return sz;
}
