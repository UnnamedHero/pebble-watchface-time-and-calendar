export default (url, type = 'GET') => new Promise((resolve) => {
  const xhr = new XMLHttpRequest();
  xhr.onload = () => resolve(xhr.responseText);
  xhr.onerror = () => {
    console.log(`error: ${JSON.stringify(xhr)}`);
    resolve(JSON.stringify({ cod: '520' }));
  };

  xhr.ontimeout = () => {
    console.log('request timeout');
    resolve(JSON.stringify({ cod: '408' }));
  };
  xhr.open(type, url, true);
  xhr.timeout = 2000;
  xhr.send();

  setTimeout(() => {
    if (xhr.readyState < 4) {
      console.log('abort request');
      xhr.abort();
      resolve(JSON.stringify({ cod: '520' }));
    }
  }, 10000);
});
