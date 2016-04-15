#!/bin/bash
#--------------------------------------------------------------------------------------------------
 # Copyright (C) 1985-2015 Intel Corporation All Rights Reserved.
 #
 # The source code, information and material ("Material") contained herein is
 # owned by Intel Corporation or its suppliers or licensors, and title to such
 # Material remains with Intel Corporation or its suppliers or licensors. The
 # Material contains proprietary information of Intel or its suppliers and
 # licensors. The Material is protected by worldwide copyright laws and treaty
 # provisions. No part of the Material may be used, copied, reproduced, modified,
 # published, uploaded, posted, transmitted, distributed or disclosed in any way
 # without Intel's prior express written permission. No license under any patent,
 # copyright or other intellectual property rights in the Material is granted to
 # or conferred upon you, either expressly, by implication, inducement, estoppel
 # or otherwise. Any license under such intellectual property rights must be
 # express and approved by Intel in writing.
 #
 # Include any supplier copyright notices as supplier requires Intel to use.
 #
 # Include supplier trademarks or logos as supplier requires Intel to use,
 # preceded by an asterisk. An asterisked footnote can be added as follows:
 # *Third Party trademarks are the property of their respective owners.
 #
 # Unless otherwise agreed by Intel in writing, you may not remove or alter
 # this notice or any other notice embedded in Materials by Intel or Intel's
 # suppliers or licensors in any way.
#--------------------------------------------------------------------------------------------------


set -ex

WORK_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
BASE_DIR="${WORK_DIR}/../../lib/android/"

echo "building libyuv library"
cd $WORK_DIR
ndk-build -j4 -B

cp -f ${WORK_DIR}/obj/local/armeabi-v7a/libyuv.a ${BASE_DIR}/armeabi-v7a/
cp -f ${WORK_DIR}/obj/local/x86/libyuv.a ${BASE_DIR}/x86/
rm -rf ${WORK_DIR}/obj

echo "finished running"
