//
// Created by Administrator on 2018/8/14.
//

#pragma once

#include "modern/locked_queue.h"

#include <stdlib.h>
#include <memory>
#include <mutex>
#include <queue>
#include <list>
#include <vector>
#include <cstring>

namespace Network {
    class MessageBuffer {
        typedef std::vector<uint8_t>::size_type size_type;

    public:
        MessageBuffer() : _wpos(0), _rpos(0), _storage() {
            _storage.resize(4096);
        }
        explicit MessageBuffer(std::size_t initialSize) : _wpos(0), _rpos(0), _storage() {
            _storage.resize(initialSize);
        }
        MessageBuffer(const void *pMsg, uint16_t wSize): _wpos(0), _rpos(0) {
            _storage.resize(wSize+1);
            Write(pMsg,wSize);
        }
        MessageBuffer(MessageBuffer const &right) : _wpos(right._wpos), _rpos(right._rpos), _storage(right._storage) {
        }

        MessageBuffer(MessageBuffer &&right) : _wpos(right._wpos), _rpos(right._rpos), _storage(right.Move()) {}

        void Reset() {
            _wpos = 0;
            _rpos = 0;
        }

        void Resize(size_type bytes) {
            _storage.resize(bytes);
        }

        uint8_t *GetBasePointer() { return _storage.data(); }

        uint8_t *GetReadPointer() { return GetBasePointer() + _rpos; }

        uint8_t *GetWritePointer() { return GetBasePointer() + _wpos; }

        void ReadCompleted(size_type bytes) { _rpos += bytes; }

        void WriteCompleted(size_type bytes) { _wpos += bytes; }

        size_type GetActiveSize() const { return _wpos - _rpos; }

        size_type GetRemainingSpace() const { return _storage.size() - _wpos; }

        size_type GetBufferSize() const { return _storage.size(); }

        // Discards inactive data
        void Normalize() {
            if (_rpos) {
                if (_rpos != _wpos)
                    memmove(GetBasePointer(), GetReadPointer(), GetActiveSize());
                _wpos -= _rpos;
                _rpos = 0;
            }
        }

        // Ensures there's "some" free space, make sure to call Normalize() before this
        void EnsureFreeSpace() {
            // resize buffer if it's already full
            if (GetRemainingSpace() == 0)
                _storage.resize(_storage.size() * 3 / 2);
        }

        void Write(void const *data, std::size_t size) {
            if (size) {
                memcpy(GetWritePointer(), data, size);
                WriteCompleted(size);
            }
        }

        std::vector<uint8_t> &&Move() {
            _wpos = 0;
            _rpos = 0;
            return std::move(_storage);
        }

        MessageBuffer &operator=(MessageBuffer const &right) {
            if (this != &right) {
                _wpos = right._wpos;
                _rpos = right._rpos;
                _storage = right._storage;
            }

            return *this;
        }

        MessageBuffer &operator=(MessageBuffer &&right) {
            if (this != &right) {
                _wpos = right._wpos;
                _rpos = right._rpos;
                _storage = right.Move();
            }

            return *this;
        }

    private:
        size_type _wpos;
        size_type _rpos;
        std::vector<uint8_t> _storage;
    };


};

