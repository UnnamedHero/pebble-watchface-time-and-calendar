export default (url, type = 'GET') => new Promise((resolve, reject) => {
  const xhr = new XMLHttpRequest();
  xhr.onload = () => {
    if (xhr.status >= 200 && xhr.status < 300) {
      resolve(xhr.responseText);
    } else {
      reject(new Error({
        status: xhr.status,
        statusText: xhr.statusText,
      }));
    }
  };
  xhr.onerror = () => {
    reject(new Error({
      status: xhr.status,
      statusText: xhr.statusText,
    }));
  };
  xhr.open(type, url);
  xhr.send();
});
