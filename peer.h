#ifndef __PEER_H__
#define __PEER_H__

#include "common.h"
#include "basic_class.h"
#include <iostream>
#include <map>

class network;
class network_udp;
class logger;
class pk_mgr;
class peer_mgr;
class logger_client;

class peer:public basic_class {
public:


	map<unsigned long, struct manifest_timmer_flag *> substream_first_reply_peer;
	map<unsigned long, struct manifest_timmer_flag *>::iterator substream_first_reply_peer_iter;
	unsigned long firstReplyPid;
	unsigned int leastSeq_set_childrenPID;

	queue<struct chunk_t *> *queue_out_ctrl_ptr;
	queue<struct chunk_t *> *queue_out_data_ptr;
	
	// TCP
	map<int, queue<struct chunk_t *> *> map_fd_out_ctrl;	// <fd, queue of chunk pointer which store outgoing control packet(chunk) >
	map<int, queue<struct chunk_t *> *> map_fd_out_data;	// <fd, queue of chunk pointer which store outgoin data packet(chunk) >
	map<unsigned long, int> map_in_pid_fd;					// store parent-peer which alreay established connection(including temp parent-peer)
	map<unsigned long, int> map_out_pid_fd;					// store child-peer which alreay established connection(including temp child-peer)
	map<int , unsigned long> map_fd_pid;					// store fd in both map_in_pid_fd and map_out_pid_fd
	map<int , Nonblocking_Buff * > map_fd_nonblocking_ctl;

	// UDP
	map<int, queue<struct chunk_t *> *> map_udpfd_out_ctrl;	// <fd, queue of chunk pointer which store outgoing control packet(chunk) >
	map<int, queue<struct chunk_t *> *> map_udpfd_out_data;	// <fd, queue of chunk pointer which store outgoin data packet(chunk) >
	map<unsigned long, int> map_in_pid_udpfd;					// store parent-peer which alreay established connection(including temp parent-peer)
	map<unsigned long, int> map_out_pid_udpfd;					// store child-peer which alreay established connection(including temp child-peer)
	map<int , unsigned long> map_udpfd_pid;					// store fd in both map_in_pid_fd and map_out_pid_fd
	map<int , Nonblocking_Buff * > map_udpfd_nonblocking_ctl;

	list<int> *fd_list_ptr;
	list<unsigned long> priority_children;					// 被 child 真正認定後將此 child 塞入 (收到 CHNK_CMD_PEER_CON)

	peer(list<int> *fd_list);
	~peer();
	void peer_set(network *net_ptr, network_udp *net_udp_ptr, logger *log_ptr , configuration *prep, pk_mgr *pk_mgr_ptr, peer_mgr *peer_mgr_ptr, logger_client * logger_client_ptr);
	void handle_connect(int sock, struct chunk_t *chunk_ptr, struct sockaddr_in cin);
	int handle_connect_request(int sock, struct level_info_t *level_info_ptr, unsigned long pid,Nonblocking_Ctl * Nonblocking_Send_Ctrl_ptr);
	void HandlePeerCon(int sock, struct chunk_request_peer_t *chunk_ptr);
	void InitPeerInfo(int sock, UINT32 peer_pid, UINT32 manifest, UINT32 my_role);
	int SendPeerCon(int sock, UINT32 peer_pid, Nonblocking_Ctl * Nonblocking_Send_Ctrl_ptr, UINT32 peercomm_session, UINT32 peer_role);
	void clear_map();
	virtual int handle_pkt_in(int sock);
	virtual int handle_pkt_in_udp(int sock);
	virtual int handle_pkt_out(int sock);
	virtual int handle_pkt_out_udp(int sock);
	virtual void handle_pkt_error(int sock);
	virtual void handle_pkt_error_udp(int sock);
	virtual void handle_sock_error(int sock, basic_class *bcptr);
	virtual void handle_job_realtime();
	virtual void handle_job_timer();

	void StopSession(unsigned long session_id);

	//////////////////////////////////////////
	//  CloseParent/CloseChild
	//             |
	//             v
	//  CloseParentUDP/CloseChildUDP
	//             |  會處理 parent/child table
	//             v
	//  CloseSocketUDP/CloseSocketTCP
	//             |  只處理 socket
	//             v
	//  network_udp->close/network->close
	//
	//  
	///////////////////////////////////////////
	void data_close(int cfd, const char *reason ,int type); 
	void peer::CloseParent(unsigned long pid, bool care, const char *reason);
	void peer::CloseParentTCP(unsigned long pid, const char *reason);
	void peer::CloseParentUDP(unsigned long pid, const char *reason);
	void peer::CloseChild(unsigned long pid, bool care, const char *reason);
	void peer::CloseChildTCP(unsigned long pid, const char *reason);
	void peer::CloseChildUDP(unsigned long pid, const char *reason);
	void peer::CloseSocketTCP(int sock, bool care, const char *reason);
	void peer::CloseSocketUDP(int sock, bool care, const char *reason);
private:
	
	int _fd;
	network *_net_ptr;
	network_udp *_net_udp_ptr;
	logger *_log_ptr;
	configuration *_prep;
	pk_mgr * _pk_mgr_ptr;
	peer_mgr * _peer_mgr_ptr;
	logger_client * _logger_client_ptr;
	int _send_byte;
	int _expect_len;
	int _offset;
//	int _time_start;
//	unsigned long _recv_byte_count;
//    unsigned long _recv_parent_byte_count;
//   unsigned long parent_manifest;

	list<int>::iterator fd_iter;
	map<int, queue<struct chunk_t *> *>::iterator map_fd_queue_iter;
	map<int , unsigned long>::iterator map_fd_pid_iter;
	map<unsigned long, int>::iterator map_pid_fd_iter;
	multimap<unsigned long, struct peer_info_t *>::iterator pid_peer_info_iter;
	map<unsigned long, struct peer_connect_down_t *>::iterator pid_peerDown_info_iter;	// store real parent-peer
	map<unsigned long, struct peer_info_t *>::iterator map_pid_child1_iter;
//	set<unsigned long>::iterator set_childrenPID_iter;
	map<int , Nonblocking_Buff * > ::iterator map_fd_nonblocking_ctl_iter;

	struct peer_info_t *peerInfoPtr ;
	struct peer_connect_down_t *peerDownInfoPtr;

	struct chunk_t *_chunk_ptr;
	struct sockaddr_in _sin, _cin;
//	struct timeb interval_time;	//--!! 0215

};

#endif





























