#!/usr/bin/env python
# -*- coding: utf-8 -*-

from ctypes import create_string_buffer
import unittest

from common import  BaseTest
from ccp_wrapper import Ccp

class TestCcp(BaseTest):

    CLASS =  Ccp
    DLL = "ccp"

    def setUp(self):
        super(TestCcp, self).setUp()
        self.obj.init()

    def testConnect(self):
        pass


def main():
    unittest.main()

if __name__ == '__main__':
    main()

