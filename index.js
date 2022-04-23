
const {
  program_init_synthesis,
  program_data_synthesis,

  program_init_control,
  program_data_control,

  program_new_frame,
  program_display,
  program_record,

  program_quit } = require('./build/Release/emit_from_cpp.node')

class Program{
  constructor() {}

  initSynthesis() {
    return new Promise((resolve) => {
      program_init_synthesis((err, result) => {
        const json = JSON.parse(result)
        resolve(json)
      })
    })
  }
  dataSynthesis(data) {
    const string = JSON.stringify(data)
    return new Promise((resolve) => {
      program_data_synthesis(string, (err, result) => {
        const json = JSON.parse(result)
        resolve(json)
      })
    })
  }

  initControl() {
    return new Promise((resolve) => {
      program_init_control((err, result) => {
        const json = JSON.parse(result)
        resolve(json)
      })
    })
  }
  dataControl(data) {
    const string = JSON.stringify(data)
    return new Promise((resolve) => {
      program_data_control(string, (err, result) => {
        const json = JSON.parse(result)
        resolve(json)
      })
    })
  }

  newFrame() {
    return new Promise((resolve) => {
      program_new_frame((err, result) => {
        const json = JSON.parse(result)
        resolve(json)
      })
    })
  }

  record() {
    return new Promise((resolve) => {
      program_record((err, result) => {
        resolve(result)
      })
    })
  }

  display(data, image) {
    const string = JSON.stringify(data)
    return new Promise((resolve) => {
      program_display(string, image, (err, result) => {
        resolve(result)
      })
    })
  }

  quit() {
    return new Promise((resolve) => {
      program_quit((err, result) => {
        resolve(result)
      })
    })
  }
}
module.exports = Program;
