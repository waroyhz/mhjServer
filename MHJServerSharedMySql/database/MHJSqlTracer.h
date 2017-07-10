/*
 * MHJSqlTracer.h
 *
 *  Created on: 2015年8月21日
 *      Author: waroy
 */

#ifndef MHJSQLTRACER_H_
#define MHJSQLTRACER_H_

#ifdef SQLITE
#include <odb/mysql/tracer.hxx>
#include <odb/mysql/database.hxx>
#include <odb/mysql/connection.hxx>
#include <odb/mysql/statement.hxx>
#endif
#include <stdio.h>

using namespace std;
#ifdef SQLITE
class MHJSqlTracer :public odb::mysql::tracer{
public:
	MHJSqlTracer(){}
//	virtual ~MHJSqlTracer();

  virtual void
  prepare (odb::mysql::connection& c, const odb::mysql::statement& s)
  {
    cerr << c.database ().db () << ": PREPARE " << "s.name()"
         << " AS " << s.text () << endl;
  }

  virtual void
  execute (odb::mysql::connection& c, const odb::mysql::statement& s)
  {
    cerr << c.database ().db () << ": EXECUTE " << "s.name ()" << endl;
  }

  virtual void
  execute (odb::mysql::connection& c, const char* statement)
  {
    cerr << c.database ().db () << ": " << statement << endl;
  }

  virtual void
  deallocate (odb::mysql::connection& c, const odb::mysql::statement& s)
  {
    cerr << c.database ().db () << ": DEALLOCATE " << "s.name ()" << endl;
  }
};
#endif
#endif /* MHJSQLTRACER_H_ */
