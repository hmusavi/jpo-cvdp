/** 
 * @file 
 * @author   Jason M. Carter (carterjm@ornl.gov)
 * @author   Aaron E. Ferber (ferberae@ornl.gov)
 * @date     May 17, 2017
 * @version  0.1
 *
 * @copyright Copyright 2017 US DOT - Joint Program Office
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * 
 * http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * Contributors:
 *    Oak Ridge National Laboratory, Center for Trustworthy Embedded Systems, UT Battelle.
 *
 * librdkafka - Apache Kafka C library
 *
 * Copyright (c) 2014, Magnus Edenhill
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met: 
 * 
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer. 
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution. 
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE 
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE 
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE 
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR 
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF 
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS 
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN 
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#include <librdkafka/rdkafkacpp.h>
#include "tool.hpp"
#include "bsmfilter.hpp"
#include "cvlib.hpp"

class PPM : public tool::Tool {

    public:

        static void sigterm (int sig);

        PPM( const std::string& name, const std::string& description );
        ~PPM();
        void metadata_print (const std::string &topic, const RdKafka::Metadata *metadata);
        bool topic_available( const std::string& topic );
        void print_configuration() const;
        bool configure();
        bool launch_consumer();
        bool launch_producer();
        RdKafka::ErrorCode msg_consume(RdKafka::Message* message, void* opaque, BSMHandler& handler);
        Quad::Ptr BuildGeofence( const std::string& mapfile );
        int operator()(void);

    private:

        static bool bsms_available;                                     ///> flag to exit application; set via signals so static.

        bool exit_eof;                                                  ///> flag to cause the application to exit on stream eof.
        int eof_cnt;                                                    ///> counts the number of eofs needed for exit_eof to work; each partition must end.
        int partition_cnt;                                              ///> TODO: the number of partitions being processed; currently 1.

        // counters.
        long bsm_recv_count;                                            ///> Counter for the number of BSMs received.
        long bsm_send_count;                                            ///> Counter for the number of BSMs published.
        long bsm_filt_count;                                            ///> Counter for hte number of BSMs filtered/suppressed.
        int64_t bsm_recv_bytes;                                         ///> Counter for the number of BSM bytes received.
        int64_t bsm_send_bytes;                                         ///> Counter for the nubmer of BSM bytes published.
        int64_t bsm_filt_bytes;                                         ///> Counter for the nubmer of BSM bytes filtered/suppressed.

        int verbosity;
        std::string mode;
        std::string debug;

        std::string brokers;
        int32_t partition;
        int64_t offset;
        std::string published_topic;                                    ///> The topic we are publishing filtered BSM to.

        // configurations; global and topic (the names in these are fixed)
        std::unordered_map<std::string, std::string> pconf;
        RdKafka::Conf *conf;
        RdKafka::Conf *tconf;

        Quad::Ptr qptr;

        std::vector<std::string> consumed_topics;                       ///> consumer topics.
        std::shared_ptr<RdKafka::KafkaConsumer> consumer;
        int consumer_timeout;
        std::shared_ptr<RdKafka::Producer> producer;
        std::shared_ptr<RdKafka::Topic> filtered_topic;
};

