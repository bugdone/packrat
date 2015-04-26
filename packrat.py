import sys
import os
import os.path
import subprocess
import time
import re
import urllib2
import bz2
import ConfigParser
from contextlib import contextmanager


cfg = {'accounts': {}}


def kill_steam():
    if os.name == 'nt':
        os.system('taskkill /im Steam.exe /F')
    else:
        os.system('killall -9 steam')


@contextmanager
def steam(user, password):
    subprocess.Popen([cfg['steam_binary'], '-silent', '-login', user, password])
    time.sleep(cfg['wait_steam'])
    yield
    kill_steam()


def download_bz2(link, dest):
    print 'Downloading', link
    compressed = urllib2.urlopen(link)
    with open(dest, 'wb') as f:
        decompressor = bz2.BZ2Decompressor()
        for data in iter(lambda : compressed.read(100 * 1024), b''):
            f.write(decompressor.decompress(data))


def download(link):
    dest = os.path.join(cfg['demo_directory'], demo_filename(link))
    download_bz2(link, dest)

    
def demo_filename(link):
    m = re.match('^http://replay(\d+).valve.net/730/([_\d]+).dem.bz2$', link)
    if m is None:
        raise Exception('Cannot parse demo link')
    return 'match730_%s_%s.dem' % (m.groups()[1], m.groups()[0])


def read_config():
    config = ConfigParser.ConfigParser()
    config.readfp(open('packrat.ini'))
    global cfg
    for key in ['steam_binary', 'demo_directory']:
        cfg[key] = config.get('general', key)
    cfg['wait_steam'] = config.getint('general', 'wait_steam')
    for k, v in config.items('accounts'):
        cfg['accounts'][k] = v


if __name__ == '__main__':
    read_config()
    kill_steam()
    for user, password in cfg['accounts'].iteritems():
        with steam(user, password):
            output = subprocess.check_output(['./broiler', cfg['demo_directory']])
            for link in output.splitlines():
                try:
                    download(link)
                except Exception, e:
                    print e
