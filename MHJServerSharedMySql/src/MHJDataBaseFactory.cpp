/*
 * mhjDB.cpp
 *
 *  Created on: 2015-7-21
 *      Author: waroy
 */

#include <MHJConfigFactory.h>
#include "MHJDataBaseFactory.h"

#include <memory>
#include <tr1/memory>
#include <tr1/shared_ptr.h>
#include "ProtocolTypeDefine.h"
#include "MHJSqlTracer.h"

#include "MoDbKey.h"
#include "MoMhjdevicebase.h"
#include "MoAdminEntity.h"
#include "JsonStrObjMap.h"
#include "MHJDBInterface.h"

//using namespace std;
//using namespace std::tr1;
using namespace boost;

#define TABLESIZE 64
#ifdef DATABASE_MYSQL
//auto_ptr<odb::mysql::connection_factory> MHJDataBaseFactory::odbfactory;
//char user[TABLESIZE];
//char passwd[TABLESIZE];
//char dbname[TABLESIZE];
//char host[TABLESIZE];
//unsigned int port;
#endif
#ifdef DATABASE_SQLITE
auto_ptr<odb::sqlite::connection_factory> MHJDataBaseFactory::odbfactory;
#endif


//TODO 需要处理数据库切换可能造成的数据库连接报错。
MHJDataBaseFactory::MHJDataBaseFactory(MHJRedisBaseFactory* redisbasefactory) {
//#ifdef DATABASE_MYSQL
//	MHJConfig config;
//	config.SetSection("Database");
//	config.ReadProfile("User", user, TABLESIZE, "mhj");
//	config.ReadProfile("Passwd", passwd, TABLESIZE, "mhj123");
//	config.ReadProfile("DBname", dbname, TABLESIZE, "mhj");
//	config.ReadProfile("Host", host, TABLESIZE, "192.168.5.15");
//	char temp[16];
//	memset(temp,0,16*sizeof(char));
//	config.ReadProfile("Port", temp, 16, "3306");
//	sscanf(temp,"%d", &port);
////	cout<<"DataBaseFactory Read configFile,  user:" <<user << "   password:"<<passwd << "  dbname: "<<dbname<<" host: "<<host<< " port: "<<port<<endl;
//
//	odbfactory = std::auto_ptr<odb::mysql::connection_factory>(
//			new odb::mysql::connection_pool_factory(20));
//
//	auto_ptr<odb::mysql::database> db(
//			new odb::mysql::database(user, passwd, dbname, host, port));
//#endif
#ifdef DATABASE_SQLITE
	odbfactory = std::auto_ptr<odb::sqlite::connection_factory>(
			new odb::sqlite::connection_pool_factory(20));

	//    sqlite database 中的函数与 mysql 存在差异，同样形式无法调用不匹配
	//	auto_ptr<odb::sqlite::database> db(
	//			new odb::sqlite::database(user, passwd, dbname, host, port));

#endif
//#ifdef DATABASE_REDIS
	redisfactory= redisbasefactory;  //new MHJRedisBaseFactory(redishost,redisport,redispasswd);
//#endif
	/********************************************
	 *
	 #ifdef DATABASE_SQLITE

	 database (const char* user,
	 const char* passwd,
	 const char* db,
	 const char* host = 0,
	 unsigned int port = 0,
	 const char* socket = 0,
	 const char* charset = 0,
	 unsigned long client_flags = 0,
	 details::transfer_ptr<connection_factory> =
	 details::transfer_ptr<connection_factory> ());
	 #endif
	 ********************************************/

	//
	//	typedef odb::query<MHJDeviceBase> query;
	//	typedef odb::result<MHJDeviceBase> result;
	//
	//	{
	//		odb::transaction t(db->begin());
	//		result r(db->query<MHJDeviceBase> (query::deviceID == 1));
	//		for (result::iterator i(r.begin()); i != r.end(); ++i) {
	//			//cout << "find, " << i->id << "!" << endl;
	//			shared_ptr<MHJDeviceBase> devicebase=i.load();
	//			cout << "find id :" << devicebase->id_ << endl;
	//			db->erase(devicebase);
	//		}
	//		t.commit();
	//	}
	//
	//	{
	//		shared_ptr<MHJDeviceBase> mhj(new MHJDeviceBase);
	//		mhj->deviceID = 1;
	//		mhj->createDate=boost::posix_time::microsec_clock::local_time();
	//		mhj->createUser="test";
	//		odb::transaction t(db->begin());
	//		db->persist(mhj);
	//		cout << "insert new id :" << mhj->id_ << endl;
	//		t.commit();
	//	}
}

MHJDataBaseFactory::~MHJDataBaseFactory() {
//#ifdef DATABASE_REDIS
	delete redisfactory;
//#endif
}

tr1::shared_ptr<MHJDeviceBase> MHJDataBaseFactory::selectDeviceSecurity(uint32_t deviceID, uint16_t idSecurity,
																		uint8_t deviceType) {
//	auto_ptr < odb::mysql::database > db(new odb::mysql::database(user, passwd, dbname, host, port, 0, 0, 0, odbfactory));
//	typedef odb::query<MHJDeviceBase> query;
//	typedef odb::result<MHJDeviceBase> result;

	std::tr1::shared_ptr < MHJDeviceBase > devicebase;
//		odb::session_required s;
//	odb::transaction t(db->begin());
//	result r(db->query < MHJDeviceBase > (query::deviceID == deviceID && query::idsecurity == idSecurity && query::deviceType == deviceType));
//	for (result::iterator i(r.begin()); i != r.end(); ++i) {
//		devicebase = i.load();
//		break;
//	}
	MHJDBInterface mhjdb;
	std::string response;
	if (mhjdb.getMhjDeviceBaseByDeviceIDIdsecurityDevictType(deviceID, idSecurity, deviceType, &response)) {
		Json::Value data = jsonGetMHJReturnData(response);
		devicebase = json2MHJDeviceBase(data);
	}
	if (!devicebase.get()) {
		printf("selectDeviceSecurity %d return null\n", deviceID);
	}

	return devicebase;
}

std::tr1::shared_ptr<MHJUserEntity> MHJDataBaseFactory::selectUserEntity(uint32_t userID) {
//	auto_ptr < odb::mysql::database > db(new odb::mysql::database(user, passwd, dbname, host, port, 0, 0, 0, odbfactory));
//#ifdef DEBUG
//	db->tracer(tracer);
//#endif
	std::tr1::shared_ptr < MHJUserEntity > userentity;
	MHJDBInterface mhjdb;
	std::string response;
	if (mhjdb.getUserEntityById(userID, &response)) {
		Json::Value data = jsonGetMHJReturnData(response);
		userentity = json2MHJUserEneity(data);
	}

//	typedef odb::query<MHJUserEntity> query;
//	typedef odb::result<MHJUserEntity> result;
//
//	shared_ptr < MHJUserEntity > userentity;
////	odb::session s;
//	odb::transaction t(db->begin());
//	result r(db->query < MHJUserEntity > ("id =" + query::_val(userID)));
//	for (result::iterator i(r.begin()); i != r.end(); ++i) {
//		userentity = i.load();
//		break;
//	}
#ifdef DEBUG
	if (userentity.get())
	{
		char tmpview[32 * 2+1];
		memset(tmpview, 0, sizeof(tmpview));
		for (int i = 0; i < 32; i++) {
			sprintf(&tmpview[i * 2], "%02x", ((BYTE*) userentity->userToken)[i]);
		}
		printf("selectUserSecurity %ld security:%s\n",userentity->id,tmpview);
	}

#endif
	if (!userentity.get()) {
		printf("selectUserSecurity %d return null\n", userID);
	}
	return userentity;
}

std::tr1::shared_ptr<MHJAdminEntity> MHJDataBaseFactory::selectAdminEntity(uint32_t adminID) {

//The Admin security data saved in database

//	auto_ptr < odb::mysql::database > db(new odb::mysql::database(user, passwd, dbname, host, port, 0, 0, 0, odbfactory));
//	typedef odb::query<MHJAdminEntity> query;
//	typedef odb::result<MHJAdminEntity> result;

	std::tr1::shared_ptr < MHJAdminEntity > adminentity;
//	odb::session s;
//	odb::transaction t(db->begin());
//	result r(db->query < MHJAdminEntity > ("id =" + query::_val(adminID)));
//	for (result::iterator i(r.begin()); i != r.end(); ++i) {
//		adminentity = i.load();
//		break;
//	}
	MHJDBInterface mhjdb;
	std::string response;
	if (mhjdb.getAdminEntityByID(adminID, &response)) {
		Json::Value data = jsonGetMHJReturnData(response);
		adminentity = json2MHJAdminEntity(data);
	}

//Save data to Redis
//#ifdef DATABASE_REDIS
//	string key = MoDbKey::getModelKey(MoAdminEntity::id(),adminID);
//	string value;
//	AdminEntityObj2JsonStr(adminentity, &value);
//	redisfactory->RedisCommand_SET((char*)key.c_str(),(char*)value.c_str());
//#endif
	if (!adminentity.get()) {
		printf("selectAdminEntity %d return null\n", adminID);
	}
	
	return adminentity;

}

std::tr1::shared_ptr<MHJDeviceChild> MHJDataBaseFactory::selectDeviceChild(uint32_t childID) {
//	auto_ptr < odb::mysql::database > db(new odb::mysql::database(user, passwd, dbname, host, port, 0, 0, 0, odbfactory));
//	typedef odb::query<MHJDeviceChild> query;
//	typedef odb::result<MHJDeviceChild> result;
//
	std::tr1::shared_ptr < MHJDeviceChild > childEntity;
//	odb::session s;
//	odb::transaction t(db->begin());
//	result r(db->query < MHJDeviceChild > (query::id == childID));
//	for (result::iterator i(r.begin()); i != r.end(); ++i) {
//		childEntity = i.load();
//		break;
//	}
	MHJDBInterface mhjdb;
	std::string response;
	if (mhjdb.getMHJDeviceChildByID(childID, &response)) {
		Json::Value data = jsonGetMHJReturnData(response);
		childEntity = json2MHJDeviceChild(data);
	}
	if (!childEntity.get()) {
		printf("selectDeviceChild %d return null\n", childID);
	}
	return childEntity;
}

//void MHJDataBaseFactory::updateDeviceBase(std::tr1::shared_ptr<MHJDeviceBase> devicebase) {
//TODO 这里需要写入到一个队列，延时批量写入数据库。
//	auto_ptr < odb::mysql::database > db(new odb::mysql::database(user, passwd, dbname, host, port, 0, 0, 0, odbfactory));
//	odb::transaction t(db->begin());
//	db->update(devicebase);
//	t.commit();

//}

std::tr1::shared_ptr<MHJDeviceActivation> MHJDataBaseFactory::selectDeviceMHJDeviceActivation(uint32_t deviceDBID) {
//	auto_ptr < odb::mysql::database > db(new odb::mysql::database(user, passwd, dbname, host, port, 0, 0, 0, odbfactory));
//	typedef odb::query<MHJDeviceActivation> query;
//	typedef odb::result<MHJDeviceActivation> result;

	std::tr1::shared_ptr < MHJDeviceActivation > deviceactivation;
	//	odb::session s;
//	odb::transaction t(db->begin());
//	result r(db->query < MHJDeviceActivation > (query::deviceID == deviceDBID));
//	for (result::iterator i(r.begin()); i != r.end(); ++i) {
//		deviceactivation = i.load();
//		break;
//	}

	MHJDBInterface mhjdb;
	std::string response;
	if (mhjdb.getMHJDeviceActivationByDeviceID(deviceDBID, &response)) {
		Json::Value data = jsonGetMHJReturnData(response);
		deviceactivation = json2MHJDeviceActivation(data);
	}
	if (!deviceactivation.get()) {
		printf("selectDeviceMHJDeviceActivation %d return null\n", deviceDBID);
	}

	return deviceactivation;
}

void MHJDataBaseFactory::selectDeviceChildMHJDeviceAuthorize(uint32_t childDBID, std::vector<std::tr1::shared_ptr<MHJDeviceAuthorize> > *vector) {
//	auto_ptr < odb::mysql::database > db(new odb::mysql::database(user, passwd, dbname, host, port, 0, 0, 0, odbfactory));
//	typedef odb::query<MHJDeviceAuthorize> query;
//	typedef odb::result<MHJDeviceAuthorize> result;

	MHJDBInterface mhjdb;
	std::string response;
	if (mhjdb.selectMHJDeviceAuthorizesByDeviceChildDBID(childDBID, &response)) {
		Json::Value data = jsonGetMHJReturnData(response);
		if (!json2MHJDeviceAuthorizeVector(data, vector)) {
			printf("selectDeviceChildMHJDeviceAuthorize %d return null\n", childDBID);
		}
	}
//	if (!deviceactivation.get()) {
//		printf("selectDeviceMHJDeviceActivation %d return null\n", deviceDBID);
//	}

//	shared_ptr < MHJDeviceAuthorize > deviceAuthorize;
////	odb::session s;
//	odb::transaction t(db->begin());
//	result r(db->query < MHJDeviceAuthorize > (query::childID == childDBID));
//	for (result::iterator i(r.begin()); i != r.end(); ++i) {
//		deviceAuthorize = i.load();
//		vector->push_back(deviceAuthorize);
////		break;
//	}
//return deviceAuthorize;
}

void MHJDataBaseFactory::updateMHJDeviceActivationsoftVer(std::tr1::shared_ptr<MHJDeviceActivation> activation) {
////TODO 这里需要写入到一个队列，延时批量写入数据库。
//	auto_ptr < odb::mysql::database > db(new odb::mysql::database(user, passwd, dbname, host, port, 0, 0, 0, odbfactory));
//	odb::transaction t(db->begin());
//	db->update(activation);
//	t.commit();
	MHJDBInterface mhjdb;
	if (!mhjdb.updateMHJDeviceActivationsoftVer(activation->id_, activation->softVer)) {
		printf("updateMHJDeviceActivationsoftVer return false\n");
	}
}

void MHJDataBaseFactory::updateDeviceChildState(std::tr1::shared_ptr<MHJDeviceChild> devicechild) {
//TODO 这里需要写入到一个队列，延时批量写入数据库。
//	auto_ptr < odb::mysql::database > db(new odb::mysql::database(user, passwd, dbname, host, port, 0, 0, 0, odbfactory));
//	odb::transaction t(db->begin());
//	db->update(devicechild);
//	t.commit();
	MHJDBInterface mhjdb;
	if (!mhjdb.updateDeviceChildState(devicechild->id_, devicechild->state)) {
		printf("updateDeviceChildState return false\n");
	}
}

//std::tr1::shared_ptr<odb::mysql::database> MHJDataBaseFactory::getDatabase() {
//	shared_ptr < odb::mysql::database > db(new odb::mysql::database(user, passwd, dbname, host, port, 0, 0, 0, odbfactory));
//	return db;
//}

void MHJDataBaseFactory::selectUserSecurityByIDList(std::vector<unsigned long>& useridList, std::vector<std::tr1::shared_ptr<MHJUserEntity> >* vector) {
//	if (useridList.size() == 0)
//		return;
//
//	auto_ptr < odb::mysql::database > db(new odb::mysql::database(user, passwd, dbname, host, port, 0, 0, 0, odbfactory));
//	typedef odb::query<MHJUserEntity> query;
//	typedef odb::result<MHJUserEntity> result;
//
//	shared_ptr < MHJUserEntity > userEntity;
//	odb::transaction t(db->begin());
//	char cids[1024];
//	sprintf(cids, "id in (");
//	std::vector<unsigned long>::iterator it;
//	int index = 0;
//	for (it = useridList.begin(); it != useridList.end(); it++) {
//		char tmp[255];
//		if (index > 0)
//			strcat(cids, ",");
//
//		sprintf(tmp, "%ld", *it);
//		strcat(cids, tmp);
//		index++;
//	}
//	strcat(cids, ")");
//
//	result r(db->query < MHJUserEntity > (cids));
//	for (result::iterator i(r.begin()); i != r.end(); ++i) {
//		userEntity = i.load();
//		vector->push_back(userEntity);
//		//		break;
//	}

	char cids[1024];
	cids[0] = 0;
//		sprintf(cids, "id in (");
	std::vector<unsigned long>::iterator it;
	int index = 0;
	for (it = useridList.begin(); it != useridList.end(); it++) {
		char tmp[255];
		if (index > 0)
			strcat(cids, ",");

		sprintf(tmp, "%ld", *it);
		strcat(cids, tmp);
		index++;
	}
//		strcat(cids, ")");

	MHJDBInterface mhjdb;
	std::string response;
	if (mhjdb.selectUserSecurityByIDList(cids, &response)) {
		Json::Value data = jsonGetMHJReturnData(response);
		if (!json2UserEntityList(data, vector)) {
			printf("selectUserSecurityByIDList size %d return null\n", (int) useridList.size());
		}
	}
}

MHJRedisBaseFactory * MHJDataBaseFactory::GetRedisBaseFactory(){
//#ifdef DATABASE_REDIS
	return redisfactory;
//#endif
}
