
#pragma once

namespace NetworkAsio {
    //暂时只用长度切包，具体包头逻辑层处理
    struct message_head {
        message_head() : length_(0) {}

        uint32_t length_;
    };

    struct MessageBuffer final {
        MessageBuffer() {}

        MessageBuffer(const char *data, uint32_t length)
                : length_(length), data_(new char[length_]) {
            memcpy(data_, data, length_);
        }

        ~MessageBuffer() {
            delete data_;
            data_ = nullptr;
        }

        uint32_t length_;
        char *data_;
    };

    inline std::shared_ptr<MessageBuffer> CreateMessage(const char *data, uint32_t length) {
        return std::make_shared<MessageBuffer>(data, length);
    }

    inline std::shared_ptr<MessageBuffer> CreateMessageWithHeader(const char *data, uint32_t length) {
        std::shared_ptr<MessageBuffer> msg = std::make_shared<MessageBuffer>();
        msg->length_ = length + sizeof(message_head);
        msg->data_ = new char[length + sizeof(message_head)];
        message_head *head = (message_head *) msg->data_;
        head->length_ = length;
        memcpy(msg->data_ + sizeof(message_head), data, length);

        return msg;
    }

};

