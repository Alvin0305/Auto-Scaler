import axios from "axios";

const API = axios.create({
  baseURL: "/work",
});

export const getStatus = async () => {
  const response = await API.get("/status");
  return response.data;
};

export const increaseLoad = async () => {
  const response = await API.post("/load/increase");
  return response.data;
};

export const decreaseLoad = async () => {
  const response = await API.post("/load/decrease");
  return response.data;
};
