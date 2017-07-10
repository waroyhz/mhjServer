//
// Created by waroy on 16-11-10.
//

#include <cstring>
#include <client_shared.h>
#include <mosquitto.h>
#include <errno.h>
#include <assert.h>
#include <pthread.h>
#include <boost/shared_ptr.hpp>
#include <MHJProtocol.h>
#include "MHJSecurityDeviceServer.h"
#include <ProtocolDeviceSwitch.h>
#include "MHJMqtt.h"

#define TopicRoot  "smart" //消息根
#define TopicDevice  "device" //设备
#define TopicUser  "user" //用户
#define Pub  "pub" //推送
#define Sub  "sub" //订阅
#define Will  "will" //遗嘱

#define DevicePubTopicRoot  TopicRoot "/"  TopicDevice  "/"  Pub  "/"
#define DeviceSubTopicRoot  TopicRoot "/"  TopicDevice  "/"  Sub  "/"
#define DeviceWillTopic  TopicRoot "/"  TopicDevice  "/"  Will  "/"

#define ACC_ReadOnly 1
#define ACC_RW 2

bool process_messages = true;
int msg_count = 0;
int publish_msg_count = 0;
struct mosquitto *pmosq = NULL;
struct mosq_config cfg;

void outMessageToSPServer(const mosquitto_message *pMessage);

SP_Server *pServer;
MHJSecurityDeviceServer *pSecurityServer;

void my_disconnect_callbark(struct mosquitto *mosq, void *obj, int ret) {
    printf("mqtt disconnect , code is %d ", ret);
}

void my_message_callback(struct mosquitto *mosq, void *obj, const struct mosquitto_message *message) {
    struct mosq_config *cfg;
    int i;
    bool res;

    if (process_messages == false) return;

    assert(obj);
    cfg = (struct mosq_config *) obj;

    if (message->retain && cfg->no_retain) return;
    if (cfg->filter_outs) {
        for (i = 0; i < cfg->filter_out_count; i++) {
            mosquitto_topic_matches_sub(cfg->filter_outs[i], message->topic, &res);
            if (res) return;
        }
    }

    if (cfg->verbose) {
        if (message->payloadlen) {
            printf("%s ", message->topic);
            fwrite(message->payload, 1, message->payloadlen, stdout);
            if (cfg->eol) {
                printf("\n");
            }
        } else {
            if (cfg->eol) {
                printf("%s (null)\n", message->topic);
            }
        }
        fflush(stdout);
    } else {
        if (message->payloadlen) {
            fwrite(message->payload, 1, message->payloadlen, stdout);
            if (cfg->eol) {
                printf("\n");
            }
            fflush(stdout);
        }
    }
    if (cfg->msg_count > 0) {
        msg_count++;
        if (cfg->msg_count == msg_count) {
            process_messages = false;
            mosquitto_disconnect(mosq);
        }
    }

    outMessageToSPServer(message);
}

void outMessageToSPServer(const mosquitto_message *message) {
    char topic[128];
    strcpy(topic, message->topic);
    char *root;
    char *clientType;
    char *action;
    char *deviceMark;
    char *protocolID;
    char *protocolVer;
//    sscanf(message->topic,"%5s/%6s/%s/%s/%s/%s",root,clientType,action,deviceMark,protocolID,protocolVer);
    root = topic;
    clientType = strstr(root, "/");
    *clientType = 0;
    clientType++;

    action = strstr(clientType, "/");
    *action = 0;
    action++;

    deviceMark = strstr(action, "/");
    *deviceMark = 0;
    deviceMark++;

    protocolID = strstr(deviceMark, "/");
    *protocolID = 0;
    protocolID++;

    protocolVer = strstr(protocolID, "/");
    *protocolVer = 0;
    protocolVer++;

    if (strcmp(root, TopicRoot) == 0 && strcmp(clientType, TopicDevice) == 0 && strcmp(action, Sub) == 0) {
//        void *protocolData =

        MHJNETProtocol recivedata;
        MHJDeviceMark mark;
        uint8_t *pmark = (uint8_t *) &mark;
        char tmp[16];
        memset(tmp, 0, sizeof(tmp));
        int tmv;
        for (int i = 0; i < 8; i++) {
            memcpy(tmp, deviceMark + i * 2, 2);
            sscanf(tmp, "%x", &tmv);
            pmark[i] = tmv;
        }
//        recivedata.device = mark;
        memcpy(&recivedata.device,&mark,sizeof(mark));
        recivedata.protocolID = atoi(protocolID);
        recivedata.ver = atoi(protocolVer);
//    protocol->time = memconv.ParseUint32(protocolTime)
        memcpy(&recivedata.time, message->payload, sizeof(recivedata.time));
        recivedata.length = message->payloadlen - sizeof(recivedata.time);
        void *data = message->payload + sizeof(recivedata.time);
//        memcpy(&recivedata.data, message->payload + sizeof(recivedata.time), recivedata.length);
        recivedata.protocolType = MPT_SWITCH; //接收到的消息类型均指定为服务器

        //除登录协议外，其他均转发给设备
        if (recivedata.protocolID!=MDPTS_LOGIN) {
            SP_Sid_t *sid = ProcessFactory::getOnlimeManager()->getSid(&recivedata.device);
            if (sid != NULL) {

                MHJProtocol *protocol = new MHJProtocol(pSecurityServer);
                shared_ptr<SP_Buffer> outBuffer = protocol->getSendBuffer(recivedata.protocolType, &recivedata.device,
                                                                          ++publish_msg_count,
                                                                          recivedata.protocolID, recivedata.ver,
                                                                          recivedata.length,
                                                                          data);
                if (outBuffer.get() != NULL) {

//    this->MHJRedisSubscribeProcessFactory::pushSPServerResponseMessage(sid, outBuffer.get());
                    SP_Response *response = new SP_Response(*sid);
                    SP_Message *outmessage = new SP_Message();
                    outmessage->getMsg()->append(outBuffer.get());
                    outmessage->getToList()->add(*sid);
                    response->addMessage(outmessage);
                    pServer->pushResponseQueue(response);
                }
                delete protocol;
            }
            else {
                printf("MQTT 消息 设备 %s 不在线\n", deviceMark);
            }
        }

//        delete []protocolData;
//        free(recivedata);
    }
}

void my_connect_callback(struct mosquitto *mosq, void *obj, int result) {
    int i;
    struct mosq_config *cfg;

    assert(obj);
    cfg = (struct mosq_config *) obj;


    if (!result) {
        mosquitto_subscribe(mosq, NULL, DeviceSubTopicRoot "#", 0);
//        for(i=0; i<cfg->topic_count; i++){
//            mosquitto_subscribe(mosq, NULL, cfg->topics[i], cfg->qos);
//        }
    } else {
        if (result && !cfg->quiet) {
            fprintf(stderr, "%s\n", mosquitto_connack_string(result));
        }
    }
}

void my_subscribe_callback(struct mosquitto *mosq, void *obj, int mid, int qos_count, const int *granted_qos) {
    int i;
    struct mosq_config *cfg;

    assert(obj);
    cfg = (struct mosq_config *) obj;

    if (!cfg->quiet) printf("Subscribed (mid: %d): %d", mid, granted_qos[0]);
    for (i = 1; i < qos_count; i++) {
        if (!cfg->quiet) printf(", %d", granted_qos[i]);
    }
    if (!cfg->quiet) printf("\n");
}

void my_log_callback(struct mosquitto *mosq, void *obj, int level, const char *str) {
    printf("%s\n", str);
}

void *mosquitto_loop_thread(void *) {
    mosquitto_loop_forever(pmosq, -1, 1);
}


int mhj_mqtt_Client_init(SP_Server *pserver, MHJSecurityDeviceServer *psecurityServer) {

//    memset(&cfg, 0, sizeof(cfg));
    pServer = pserver;
    pSecurityServer = psecurityServer;

    int rc;
    rc = client_config_load(&cfg, CLIENT_SUB, NULL, NULL);
//	rc = client_config_load(&cfg, CLIENT_SUB, argc, argv);
//	if(rc){
//		client_config_cleanup(&cfg);
//		if(rc == 2){
//			/* --help */
//			print_usage();
//		}else{
//			fprintf(stderr, "\nUse 'mosquitto_sub --help' to see usage.\n");
//		}
//		return 1;
//	}

    mosquitto_lib_init();

//	if(client_id_generate(&cfg, "mosqsub")){
//		return 1;
//	}

    cfg.id = "0001000000020000";
    cfg.username = cfg.id;
    cfg.password = "1713644b3b9caae10a3da1b2aa6a05f3";
    cfg.host = "192.168.5.150";
    cfg.port = 8883;
//    cfg.no_retain = true;
    cfg.verbose = true;
    cfg.keepalive=60;
//    cfg.debug = true;

    pmosq = mosquitto_new(cfg.id, cfg.clean_session, &cfg);
    if (!pmosq) {
        switch (errno) {
            case ENOMEM:
                if (!cfg.quiet) fprintf(stderr, "Error: Out of memory.\n");
                break;
            case EINVAL:
                if (!cfg.quiet) fprintf(stderr, "Error: Invalid id and/or clean_session.\n");
                break;
        }
        mosquitto_lib_cleanup();
        return 1;
    }
    if (client_opts_set(pmosq, &cfg)) {
        return 1;
    }
    if (cfg.debug) {
        mosquitto_log_callback_set(pmosq, my_log_callback);
        mosquitto_subscribe_callback_set(pmosq, my_subscribe_callback);
    }
    mosquitto_connect_callback_set(pmosq, my_connect_callback);
    mosquitto_message_callback_set(pmosq, my_message_callback);

    mosquitto_disconnect_callback_set(pmosq, my_disconnect_callbark);


    rc = client_connect(pmosq, &cfg);
    if (rc) return rc;

//    pthread_t pthreadid;
//    int ret = pthread_create(&pthreadid, NULL,  mosquitto_loop_thread, NULL);
//    rc = mosquitto_loop_forever(pmosq, -1, 1);
    rc = mosquitto_loop_start(pmosq);


    return rc;
}

void mhj_mqtt_Client_close() {
    mosquitto_destroy(pmosq);
    mosquitto_lib_cleanup();

}

void mhj_mqtt_publish_Protocol(boost::shared_ptr<MHJProtocol> ptr) {
    char strmark[32];
    sprintMark(strmark, ptr->device());
    char topic[128];
    sprintf(topic, "%s%s/%d/%d", DevicePubTopicRoot, strmark, ptr->protocolID(), ptr->Ver());

    SP_Buffer buff;
    DWORD t = ptr->Time();
    buff.append(&t, sizeof(t));
    buff.append(ptr->data(), ptr->length());

    int mid;
    mosquitto_publish(pmosq, &mid, topic, buff.getSize(), buff.getBuffer(), 0, false);
}
