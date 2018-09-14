export default (url, type = 'GET') => new Promise((resolve) => {
  const xhr = new XMLHttpRequest();
  xhr.timeout = 10000;
  xhr.onload = () => resolve(xhr.responseText);
  xhr.onerror = () => {
    // console.log(`error: ${JSON.stringify(xhr)}`);
    resolve(JSON.stringify({ cod: '520' }));
  };
  xhr.ontimeout = () => resolve(JSON.stringify({ cod: '408' }));
  xhr.open(type, url, true);
  xhr.send();
});
