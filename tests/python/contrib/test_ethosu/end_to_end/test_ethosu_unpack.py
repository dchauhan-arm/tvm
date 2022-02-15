# Licensed to the Apache Software Foundation (ASF) under one
# or more contributor license agreements.  See the NOTICE file
# distributed with this work for additional information
# regarding copyright ownership.  The ASF licenses this file
# to you under the Apache License, Version 2.0 (the
# "License"); you may not use this file except in compliance
# with the License.  You may obtain a copy of the License at
#
#   http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing,
# software distributed under the License is distributed on an
# "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
# KIND, either express or implied.  See the License for the
# specific language governing permissions and limitations
# under the License.
# pylint: disable=invalid-name, unused-argument
import pytest

pytest.importorskip("ethosu.vela")

import tensorflow as tf
from tests.python.contrib.test_ethosu.end_to_end import comparison_infra


ACCEL_TYPES = ["ethos-u55-256", "ethos-u55-128", "ethos-u55-64", "ethos-u55-32", "ethos-u65-256"]


@pytest.mark.parametrize("accel_type", ACCEL_TYPES)
@pytest.mark.parametrize(
    "ifm_shape,axis",
    [[(1, 2, 3, 4), 1], [(2, 3), 1], [(5, 6, 7), 2]],
)
def test_tflite_unpack(accel_type, ifm_shape, axis):
    @tf.function
    def unpack_func(x):
        return tf.unstack(x, axis=axis)

    comparison_infra._compare_tvm_with_tflite(unpack_func, [ifm_shape], accel_type)


if __name__ == "__main__":
    pytest.main([__file__])
