import {
  spawnSync
} from 'child_process'
import {
  showMessage
} from '../../utils/helpers.js'

import ProcessesList from './ProcessesList'

export default {
  template: `<transition name="slide-fade">
                <div class="content">
						      <slot></slot>
                  <div class="processes">
                  <div class="fl w100">
                      <h3 class="fl">{{ lang('process') }} ({{ processes.length }})</h3>
                      <input type="text" v-model="searchString" :placeholder="lang('search') + '...'" />
                  </div>
                  <table class="datatable">
                    <thead>
                      <tr>
                        <th @click="sortPid" scope="col" width="80">
                          PID <i :class="sortAmount(sPid)"></i>
                        </th>
                        <th @click="sortCpu" scope="col" width="70">
                          %CPU <i :class="sortAmount(sCpu)"></i>
                        </th>
                        <th @click="sortMem" scope="col" width="80">
                          %MEM <i :class="sortAmount(sMem)"></i>
                        </th>
                        <th @click="sortRss" scope="col" width="80">
                          RSS <i :class="sortAmount(sRss)"></i>
                        </th>
                        <th @click="sortVSize" scope="col" width="80">
                          VSIZE <i :class="sortAmount(sVSize)"></i>
                        </th>
                        <th @click="sortUser" scope="col" width="100">
                          User <i :class="sortAmount(sUser)"></i>
                        </th>
                        <th @click="sortCmd" scope="col">
                          Process <i :class="sortAmount(sCmd)"></i>
                        </th>
                      </tr>
                    </thead>
                  </table>
                  <div class="tbody scroll">
                    <table class="datatable" style="margin-top: -18px;">
                      <thead>
                        <tr>
                          <th scope="col" width="80"></th>
                          <th scope="col" width="70"></th>
                          <th scope="col" width="80"></th>
                          <th scope="col" width="80"></th>
                          <th scope="col" width="80"></th>
                          <th scope="col" width="100"></th>
                          <th scope="col"></th>
                        </tr>
                      </thead>
                      <tbody>
                      <tr v-for="p in processes" @click="selectPid(p.pid)" :class="(selectedPid == p.pid) ? 'selectedRow' : ' '">
                          <td :title="p.pid">{{ p.pid }}</th>
                          <td :title="p.pcpu">{{ p.pcpu }}</td>
                          <td :title="p.pmem">{{ p.pmem }}</td>
                          <td :title="p.rss">{{ p.rss }}</td>
                          <td :title="p.vsize">{{ p.vsize }}</td>
                          <td :title="p.uname">{{ p.uname }}</td>
                          <td :title="p.cmd">{{ p.cmd }}</td>
                      </tr>
                      </tbody>
                    </table>
                  </div>
                  <button @click="endProcess" :disabled="selectedPid == 0">{{ lang('endProcess') }}</button>
                </div>
              </div>
            </transition>`,
  data() {
    return ({
      processes: [],
      sPid: 0,
      sMem: 0,
      sRss: 0,
      sVSize: 0,
      sUser: 0,
      sCmd: 0,
      sCpu: 1,
      sort: ['--sort', '-pcpu'],
      searchString: '',
      selectedPid: 0
    })
  },
  created() {
    setInterval(() => this.processes = ProcessesList(this.searchString, this.sort), 1000)
  },
  methods: {
    selectPid(pid) {
      if (this.selectedPid == pid)
        this.selectedPid = 0
      else
        this.selectedPid = pid
    },
    sortPid() {
      this.sPid = this.changeAmount(this.sPid)
      this.changeSort(this.sPid, 'pid')
    },
    sortCpu() {
      this.sCpu = this.changeAmount(this.sCpu)
      this.changeSort(this.sCpu, 'pcpu')
    },
    sortMem() {
      this.sMem = this.changeAmount(this.sMem)
      this.changeSort(this.sMem, 'pmem')
    },
    sortRss() {
      this.sRss = this.changeAmount(this.sRss)
      this.changeSort(this.sRss, 'rss')
    },
    sortVSize() {
      this.sVSize = this.changeAmount(this.sVSize)
      this.changeSort(this.sVSize, 'vsize')
    },
    sortUser() {
      this.sUser = this.changeAmount(this.sUser)
      this.changeSort(this.sUser, 'uname')
    },
    sortCmd() {
      this.sCmd = this.changeAmount(this.sCmd)
      this.changeSort(this.sCmd, 'cmd')
    },
    sortAmount(s) {
      switch (s) {
        case 1:
          return 'icon-sort-desc'
        case -1:
          return 'icon-sort-asc'
        default:
          return ''
      }
    },
    changeSort(column, header) {
      this.sort = column == 0 ? [] : ['--sort', column == 1 ? '-' + header : header]
    },
    changeAmount(column) {
      this.sPid = this.sMem = this.sRss = this.sVSize = this.sUser = this.sCmd = this.sCpu = 0
      return column == 1 ? -1 : (column == 0 ? 1 : 0)
    },
    endProcess() {
      if (this.selectedPid != 0) {
        var kill = spawnSync('kill', [this.selectedPid])

        if (kill.stderr.toString().indexOf('permitted') !== -1)
          showMessage(lang('permitted'), 'error')
        else
          logger.error('Process Kill', kill.stderr.toString())
      }
      this.selectedPid = 0
    }
  }
}