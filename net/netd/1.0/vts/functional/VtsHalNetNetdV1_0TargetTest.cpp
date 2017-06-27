/*
 *Copyright (c) 2017, The Linux Foundation. All rights reserved.
 *
 *Redistribution and use in source and binary forms, with or without
 *modification, are permitted provided that the following conditions are
 *met:
 *   * Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *   * Redistributions in binary form must reproduce the above
 *     copyright notice, this list of conditions and the following
 *     disclaimer in the documentation and/or other materials provided
 *     with the distribution.
 *   * Neither the name of The Linux Foundation nor the names of its
 *     contributors may be used to endorse or promote products derived
 *     from this software without specific prior written permission.
 *
 *THIS SOFTWARE IS PROVIDED "AS IS" AND ANY EXPRESS OR IMPLIED
 *WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 *MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT
 *ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS
 *BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 *CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 *SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
 *BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 *WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
 *OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN
 *IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#define LOG_TAG "netd_hidl_test"

#include <android/system/net/netd/1.0/INetd.h>
#include <log/log.h>
#include <VtsHalHidlTargetTestBase.h>


using ::android::system::net::netd::V1_0::INetd;
using ::android::hardware::Return;
using ::android::sp;

class NetdHidlTest : public ::testing::VtsHalHidlTargetTestBase {
public:
    virtual void SetUp() override {
        netd = ::testing::VtsHalHidlTargetTestBase::getService<INetd>();
        ASSERT_NE(nullptr, netd.get()) << "Could not get HIDL instance";
    }

    sp<INetd> netd;
};

// positive test. Ensure netd creates an oem network and returns valid netHandle, and destroys it.
TEST_F(NetdHidlTest, TestCreateAndDestroyOemNetworkOk) {
    auto cb = [this](uint64_t netHandle,
            uint32_t packetMark, INetd::StatusCode status) {

        ASSERT_EQ(INetd::StatusCode::OK, status);
        ASSERT_NE((uint64_t)0, netHandle);
        ASSERT_NE((uint32_t)0, packetMark);

        Return<INetd::StatusCode> retStatus = netd->destroyOemNetwork(netHandle);
        ASSERT_EQ(INetd::StatusCode::OK, retStatus);
    };

    Return<void> ret = netd->createOemNetwork(cb);
    ASSERT_TRUE(ret.isOk());
}

// negative test. Ensure destroy for invalid OEM network fails appropriately
TEST_F(NetdHidlTest, TestDestroyOemNetworkInvalid) {
    uint64_t nh = 0x6600FACADE;

    Return<INetd::StatusCode> retStatus = netd->destroyOemNetwork(nh);
    ASSERT_EQ(INetd::StatusCode::INVALID_ARGUMENTS, retStatus);
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    int status = RUN_ALL_TESTS();
    ALOGE("Test result with status=%d", status);
    return status;
}
