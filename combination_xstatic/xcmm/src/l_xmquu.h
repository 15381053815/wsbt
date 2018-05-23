#ifndef __L_XMQUU_H_
#define __L_XMQUU_H_

#include "l_xlock.h"
typedef struct __slqmsg{
	int		format;
	int		ret;
	int		im[6];

	void*	pm[4];
	double  dm[2];
	char	cm[2][32];
}lqmsg;

class lxquu
{
public:	
	lxquu();
	virtual ~lxquu();
public:
	void	lqpshinmsg(lqmsg &mn, bool brep = false);
	int		lqgetinmsg(lqmsg &mn, bool bdel = true);
	void	lqclrinmsg();

	void	lqpshotmsg(lqmsg &mn, bool brep = false);
	int		lqgetotmsg(lqmsg &mn, bool bdel = true);
	void	lqclrotmsg();

	int		lqgetotmsgsize();
	int		lqgetinmsgsize();
private:
	void*	_lstin;
	lxlock	_lkin;

	void*	_lstot;
	lxlock	_lkot;
};

#endif