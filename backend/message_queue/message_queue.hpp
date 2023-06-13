#pragma once

#include <sys/ipc.h>
#include <sys/msg.h>

#include <iostream>
#include <optional>

#define MSG_SIZE 22
#define PERM 0666

namespace MQ {

enum Waiting { Wait, DontWait };

class MessageQueue {
 public:
  MessageQueue() = default;

  explicit MessageQueue(key_t key);

  MessageQueue(const MessageQueue&) noexcept;

  ~MessageQueue();

  MessageQueue& operator=(const MessageQueue&) noexcept;

  std::optional<int> Receive(int64_t msg_type, Waiting wait);

  void Send(int message, int64_t msg_type);

 private:
  int descriptor_ = -1;
  size_t* num_of_class_objects_ = nullptr;

  struct MsgBuf {
    int64_t type;
    char msg[MSG_SIZE];
  };

  void DeleteQueue() noexcept;
};

}  // namespace MQ