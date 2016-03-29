#!/usr/bin/env python
# -*- coding: utf-8 -*-

import sys
import os
reload(sys)
sys.setdefaultencoding('utf-8')

CONF_DIR = 'tor'
CONF_DIR_POLIPO = 'polipo'

if not os.path.exists(CONF_DIR):
    os.mkdir(CONF_DIR)

if not os.path.exists(CONF_DIR_POLIPO):
    os.mkdir(CONF_DIR_POLIPO)

def main(numOfInstances=4):
    """TODO: Docstring for main.

    :numOfInstances: TODO
    :returns: TODO

    """
    conf_tpl = """
SOCKSPort {} # Default: Bind to localhost:9050 for local connections.
SocksListenAddress      127.0.0.1

VirtualAddrNetworkIPv4 10.0.0.0/10
AutomapHostsOnResolve 1
TransPort {}

MaxCircuitDirtiness     30
RunAsDaemon 1

Log notice file {}
    """

    run_tpl = """
tor -f {}
    """

    conf_tpl_polipo = """
proxyAddress = "0.0.0.0"    # IPv4 only
proxyPort = {}
socksParentProxy = "localhost:{}"
socksProxyType = socks5
authCredentials=username:password
    """
    run_tpl_polipo = """
tor -f {}
    """
    print('generating conf for {} instances of tor'.format(numOfInstances))
    with open('{}/run.sh'.format(CONF_DIR), 'w') as f:
        f.write('#!bin/sh\n\n')
    with open('{}/run.sh'.format(CONF_DIR_POLIPO), 'w') as f:
        f.write('#!bin/sh\n\n')
    for i in range(numOfInstances):
        # generating tor conf
        with open('{}/{}'.format(CONF_DIR, 'torrc.%s'%i), 'w') as f:
            f.write(conf_tpl.format(9050 + i,
                                    8740 + i,
                                    '/var/log/tor/notices.log.%s'%i))

        with open('{}/run.sh'.format(CONF_DIR), 'a') as f:
            f.write(run_tpl.format('./torrc.%s'%i))

        # generating polipo conf for redirecting traffics to socks5 proxy
        # that tor listen on
        with open('{}/{}'.format(CONF_DIR_POLIPO, 'config.%s'%i), 'w') as f:
            f.write(conf_tpl_polipo.format(8123 + i, 9050 + i))
        with open('{}/run.sh'.format(CONF_DIR_POLIPO), 'a') as f:
            f.write(run_tpl_polipo.format('./torrc.%s'%i))

    os.chmod('{}/run.sh'.format(CONF_DIR), 0755)
    os.chmod('{}/run.sh'.format(CONF_DIR_POLIPO), 0755)
    print('Now you can run run.sh in tor and polipo directory to start your proxy service!')


if __name__ == "__main__":
    if len(sys.argv) > 1:
        main(numOfInstances=int(sys.argv[1]))
    else:
        main()

