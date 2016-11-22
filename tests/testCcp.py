#!/usr/bin/env python
# -*- coding: utf-8 -*-

from ctypes import create_string_buffer
import unittest

from common import  BaseTest
#from msg import Messaging
from ccp_wrapper import Ccp

class TestUtlStringFunctions(BaseTest):

    CLASS =  Ccp
    DLL = "ccp"

    def testConnect(self):
        pass
#        dest = create_string_buffer(120)
#        src = create_string_buffer(b"")
#        self.obj.utl.strCpy(dest, src)
#        self.assertEqual(dest.value, "")


def main():
    unittest.main()

if __name__ == '__main__':
    main()

