const { spawn } = require('child_process');
const Promise = require('promise');

/**
 * cryptpipe object constructor.
 *
 * Supports two methods:
 *   crypt(key, salt) => Promise with results
 *     Performs encryption

 *   end() => void
 *     Closes the subprocess. Calling crypt() after a call to end()
 *     is an error
 *
 * @param executable  Path to cryptpipe executable
 */
function cryptPipe(executable) {
    let cpProcess = spawn('../../cryptpipe', [],
        { stdio: ['pipe', 'pipe', process.stderr] });
    let q = [];
    let inputBuffer = '';
    cpProcess.stdout.on('data', gotData);
    return { crypt, end };

    function crypt(key, salt) {
        let ret = new Promise((fulfill, reject) => q.push({
            fulfill,
            reject
        }));
        cpProcess.stdin.cork();
        cpProcess.stdin.write(key);
        cpProcess.stdin.write('\n');
        cpProcess.stdin.write(salt);
        cpProcess.stdin.write('\n');
        process.nextTick(() => cpProcess.stdin.uncork());
        return ret;
    }

    function end() {
        cpProcess.stdin.end();
    }

    function gotData(data) {
        inputBuffer += data;
        let nlp = inputBuffer.indexOf('\n');
        if (nlp>=0) {
            let r = inputBuffer.slice(0,nlp);
            inputBuffer = inputBuffer.slice(nlp+1);
            let future = q.splice(0,1)[0];
            if (r === '!') {
                future.reject();
            } else {
                future.fulfill(r);
            }
        }
    }
}

let cp = cryptPipe("../../cryptpipe");
cp.crypt("salad","$6$pineapple")
    .then(ret => {
        console.log(ret);
    });
cp.crypt("salad","$6$banana")
    .then(ret => {
        console.log(ret);
    })
    .finally(() => cp.end());
