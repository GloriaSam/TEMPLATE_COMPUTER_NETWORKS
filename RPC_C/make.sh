#!/bin/bash

rpcgen *.x
gcc -g -o Client *_Client.c *_clnt.c *_xdr.c
gcc -g -o Server *_Server.c *_svc.c *_xdr.c
