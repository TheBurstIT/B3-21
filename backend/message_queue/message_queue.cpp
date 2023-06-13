#include "message_queue.hpp"

namespace MQ {

MessageQueue::MessageQueue(key_t key) {
  descriptor_ = msgget(key, PERM);
  if (descriptor_ == -1) {
    if (errno == ENOENT) {
      descriptor_ = msgget(key, IPC_CREAT | PERM);
      if (descriptor_ < 0) {
        throw errno;
      }
    } else {
      throw errno;
    }
  }

  try {
    num_of_class_objects_ = new size_t(1);
  } catch (...) {
    DeleteQueue();
    throw;
  }
}

MessageQueue::MessageQueue(const MQ::MessageQueue& other) noexcept
    : descriptor_(other.descriptor_),
      num_of_class_objects_(other.num_of_class_objects_) {
  if (num_of_class_objects_ != nullptr) {
    *num_of_class_objects_ += 1;
  }
}

MessageQueue::~MessageQueue() {
  if (num_of_class_objects_ != nullptr) {
    *num_of_class_objects_ -= 1;
    if (*num_of_class_objects_ == 0) {
      DeleteQueue();
      delete num_of_class_objects_;
    }
  }
}

MessageQueue& MessageQueue::operator=(const MQ::MessageQueue& other) noexcept {
  // очищаем старую очередь (если надо)
  if (num_of_class_objects_ != nullptr) {
    *num_of_class_objects_ -= 1;
    if (*num_of_class_objects_ == 0) {
      DeleteQueue();
      delete num_of_class_objects_;
    }
  }

  descriptor_ = other.descriptor_;
  num_of_class_objects_ = other.num_of_class_objects_;

  if (num_of_class_objects_ != nullptr) {
    *num_of_class_objects_ += 1;
  }
  return *this;
}

std::optional<int> MessageQueue::Receive(int64_t msg_type, Waiting wait) {
  MsgBuf msg;
  int flag = wait == Wait ? 0 : IPC_NOWAIT;
  if (msgrcv(descriptor_, &msg, MSG_SIZE, msg_type, flag) < 0) {
    if (errno == ENOMSG) {
      return {};
    }
    throw errno;
  }
  int result;
  sscanf(msg.msg, "%d", &result);
  return result;
}

void MessageQueue::Send(int message, int64_t msg_type) {
  MsgBuf msg_buf;
  sprintf(msg_buf.msg, "%d", message);
  msg_buf.type = msg_type;
  if (msgsnd(descriptor_, &msg_buf, strlen(msg_buf.msg) + 1, 0) < 0) {
    throw errno;
  }
}

void MessageQueue::DeleteQueue() noexcept {
  msgctl(descriptor_, IPC_RMID, nullptr);
}

}