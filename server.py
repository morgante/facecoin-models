from time import sleep
import tornado
from tornado.httpserver import HTTPServer
from tornado.ioloop import IOLoop
from tornado.web import Application, asynchronous, RequestHandler
from multiprocessing.pool import ThreadPool
import os

import urllib2 as urllib

import logging

logging.basicConfig(level=logging.DEBUG,filename="py.log")
 
_workers = ThreadPool(10)
 
def run_background(func, callback, args=(), kwds={}):
    def _callback(result):
        IOLoop.instance().add_callback(lambda: callback(result))
    _workers.apply_async(func, args, kwds, _callback)
 
# blocking task like querying to MySQL
def blocking_task(url):

    import urllib
    import os
    import time

    url = 'https://www.filepicker.io/api/file/V5Wss9Tq6VkCyFlwlSAa'

    png = url + '/convert?format=png&w=500'

    # This code is all super hacky
    opener=urllib.URLopener()
    opener.retrieve(png, "face.png")

    # Clean it up
    os.system("convert face.png -sharpen 0x2 +dither -shade 0x45 -colors 3 -colorspace gray -normalize -despeckle -noise 5 face2.png")

    # Make it into an OBJ
    os.system("/app/anneal face2.png btc.png facecoin.obj")

    # Store
    filename = str(time.time()) + ".obj"

    os.system("mv facecoin.obj obj/%s" % str(filename))

    return filename;
 
class Handler(RequestHandler):
    @asynchronous
    def get(self):
    	url = self.get_argument("url", None, "")

        run_background(blocking_task, self.on_complete, (url,))
 
    def on_complete(self, res):
        self.write(res);
        self.finish()

settings = dict(
    static_path=os.path.join(os.path.dirname(__file__), "obj"),
    debug=True
)

handlers = [("/", Handler), (r"/(.*)", tornado.web.StaticFileHandler, {"path": "./obj"})]

application = Application(handlers, **settings)

HTTPServer(application).listen(5000)
IOLoop.instance().start()