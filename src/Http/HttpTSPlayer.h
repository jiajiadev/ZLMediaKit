﻿/*
 * Copyright (c) 2020 The ZLMediaKit project authors. All Rights Reserved.
 *
 * This file is part of ZLMediaKit(https://github.com/xia-chu/ZLMediaKit).
 *
 * Use of this source code is governed by MIT license that can be found in the
 * LICENSE file in the root of the source tree. All contributing project authors
 * may be found in the AUTHORS file in the root of the source tree.
 */

#ifndef HTTP_HTTPTSPLAYER_H
#define HTTP_HTTPTSPLAYER_H

#include "Http/HttpDownloader.h"
#include "Player/MediaPlayer.h"
#include "Rtp/TSDecoder.h"

using namespace toolkit;

namespace mediakit {

//http-ts播发器，未实现ts解复用
class HttpTSPlayer : public HttpClientImp {
public:
    using Ptr = std::shared_ptr<HttpTSPlayer>;
    using onComplete = std::function<void(const SockException &)>;

    HttpTSPlayer(const EventPoller::Ptr &poller = nullptr, bool split_ts = true);
    ~HttpTSPlayer() override = default;

    /**
     * 设置下载完毕或异常断开回调
     */
    void setOnComplete(onComplete cb);

    /**
     * 设置接收ts包回调
     */
    void setOnPacket(TSSegment::onSegment cb);

protected:
    ///HttpClient override///
    ssize_t onResponseHeader(const string &status, const HttpHeader &header) override;
    void onResponseBody(const char *buf, size_t size, size_t recved_size, size_t total_size) override;
    void onResponseCompleted() override;
    void onDisconnect(const SockException &ex) override;

protected:
    /**
     * 收到ts数据
     */
    virtual void onPacket(const char *data, size_t len);

private:
    void emitOnComplete(const SockException &ex);

private:
    //是否为mpegts负载
    bool _is_ts_content = false;
    //第一个包是否为ts包
    bool _is_first_packet_ts = false;
    //是否判断是否是ts并split
    bool _split_ts;
    string _status;
    TSSegment _segment;
    onComplete _on_complete;
    TSSegment::onSegment _on_segment;
};

}//namespace mediakit
#endif //HTTP_HTTPTSPLAYER_H
