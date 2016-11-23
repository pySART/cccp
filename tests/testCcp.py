#!/usr/bin/env python
# -*- coding: utf-8 -*-

from ctypes import create_string_buffer
import unittest

from common import  BaseTest
from ccp_wrapper import Ccp, LocalTransport

from pyccp import ccp
from pyccp.master import Master

def sender(cmo):
    print("CMO: {0}".format(cmo))

class TestCcp(BaseTest):

    CLASS =  Ccp
    DLL = "ccp"

#    def setUp(self):
#        super(TestCcp, self).setUp()
#        transport = LocalTransport(self.obj)
#        self.master = Master(transport)

    def runTest(self, func, *params):
        getattr(self.master, func)(*params)
        #result = str(self.master.transport.message)
        #self.assertEqual(result, expectedResult)

    def testConnect(self):
        #self.obj.setSendCallout(sender)
        transport = LocalTransport(self.obj)
        self.master = Master(transport)
        self.obj.init()
        self.runTest("connect", 0x7E1, 0x39)

    @unittest.skip
    def testInitWorksAsExpected(self):
        self.obj.init()
        self.assertEqual(self.obj.getMta(), 0)
        self.assertEqual(self.obj.getConnectionState(), 0)

    def testCallout(self):
        #self.obj.setSendCallout(sender)
        pass


def main():
    unittest.main()

if __name__ == '__main__':
    main()

